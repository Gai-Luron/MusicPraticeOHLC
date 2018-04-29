#include <QObject>
#include <QTimer>
#include "processfile.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QSemaphore>
#include <QList>




#define CHANNELS 2
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER SAMPLE_RATE/100 // HOW MANY FRAME TOO QUERY IN CALLBACK. LESS IS, LESS DELAY TO START
#define FRAMES_PER_BUFFER_IN_MEMORY SAMPLE_RATE*2 // FRAME IN BUFFER MEMORY IN SECOND
#define CURRTEMPO   (100)
#define CURRSEMITONE   (0)
#define TIMERTIMEOUT (400) // CALLBACK ON TIMER FOR INSERT INTO MEMORY BUFFER
#define SIZEMEMBUFFER FRAMES_PER_BUFFER_IN_MEMORY*CHANNELS*10

QElapsedTimer elapsedTimer;
QSemaphore qSem(1);


float data [FRAMES_PER_BUFFER_IN_MEMORY*CHANNELS] ;
float data2 [FRAMES_PER_BUFFER_IN_MEMORY*CHANNELS] ;

struct structTimerBuff {
                bool isAvailable=false;
                bool endOfFileReached = false;
                unsigned long idxNext = 0;
                float buffer[SIZEMEMBUFFER]; // Receive in Memory Structure
};

structTimerBuff MainBuffer;

////////////////////////////////////////////////////////////////////////////
/// \brief processFile::paCallback
/// \param inputBuffer
/// \param outputBuffer
/// \param framesPerBuffer
/// \param timeInfo
/// \param statusFlags
/// \param userData
/// \return
////////////////////////////////////////////////////////////////////////////
int processFile::paCallback( const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData
                       )

{
    // Remove Warning unused var in compiler
    (void)inputBuffer;
    (void)timeInfo;
    (void)statusFlags;


    processFile *localPF;
    unsigned long countRead;
    float *out = (float*)outputBuffer;
    if( MainBuffer.isAvailable ){
        localPF = (processFile*)userData;
        countRead = localPF->getMemBuffer( out,framesPerBuffer );
        if( countRead < framesPerBuffer ){
            return paComplete;
        }
    }
    else
        memset( out,0,framesPerBuffer*CHANNELS*sizeof(float));

    return paContinue;
}

////////////////////////////////////////////////////////////////////////////
/// \brief processFile::processFile
////////////////////////////////////////////////////////////////////////////
processFile::processFile()
    {
    PaError err;


    currTempo = CURRTEMPO;
    currSemiTone = CURRSEMITONE;

    bypassStrech = false; // if set to true, prog don't use soundTouch library, no sound streching



    // Intitialize Sountouche values
    pSoundTouch.setChannels(CHANNELS);
    pSoundTouch.setSampleRate(SAMPLE_RATE);
    pSoundTouch.setTempo(currTempo/100 );
    pSoundTouch.setPitchSemiTones(currSemiTone);

// Intitialize and Start PortAudio
    err = Pa_Initialize();
    if( err != paNoError ){
        qDebug() <<  "PortAudio error: " << Pa_GetErrorText( err );
        goto error;
    }


    return;
error:
  Pa_Terminate();
  qDebug() << "An error occured while using the portaudio stream" ;
  qDebug() << "Error number: " << err ;
  qDebug() << "Error message: " << Pa_GetErrorText( err ) ;
  return ;
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::~processFile
////////////////////////////////////////////////////////////////////////////
processFile::~processFile()
{
    sf_close(file);
    qDebug() << "Destruction";
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::openSoundFile
/// \param fileName
/// \return
////////////////////////////////////////////////////////////////////////////
long long processFile::openSoundFile(QString fileName)
    {
    PaError err;
    PaStreamParameters outputParameters;
    PaStreamFlags streamFlags;
    PaDeviceIndex outDevNum;

    currFileName = fileName;
    SF_INFO sfinfo;
    sfinfo.channels = CHANNELS;
    sfinfo.samplerate = SAMPLE_RATE;
    if( isfileOpen )
        sf_close( file );
    LPCWSTR LfileName = (LPCWSTR)currFileName.utf16();
//  file = sf_open( currFileName.toStdString().c_str(),SFM_READ,&sfinfo );
    file =  sf_wchar_open( LfileName, SFM_READ, &sfinfo);
    Q_ASSERT(file != NULL );
    isfileOpen = true;
    totalFrames = 0;

    while (true) {

        long long countFrameReadFile = sf_readf_float(file, data, SAMPLE_RATE); // Put One second each time
        if( countFrameReadFile == 0)
            break;
        totalFrames += countFrameReadFile;
    }

    sf_seek(file,0,SEEK_SET);



// Initialize SoundTouch
    pSoundTouch.clear();
// Initialize Buffer
    MainBuffer.isAvailable = false;
    MainBuffer.idxNext = 0;
    MainBuffer.endOfFileReached = false;
    currFileFrameRead = 0;
    currPlayFrameRead = 0;
    currPlayedFrameRead = 0;

    listOfBlock.clear();

    err = Pa_IsStreamStopped(stream);
    if( err == 0){
        err = Pa_StopStream( stream );
        if( err != paNoError )
            goto error;
        err = Pa_CloseStream( stream );
        if( err != paNoError )
            goto error;
    }
    memset( &outputParameters,0, sizeof( outputParameters ) ); //not necessary if you are filling in all the fields
    outDevNum = Pa_GetDefaultOutputDevice();
    outputParameters.device = outDevNum; // default output device
    if (outputParameters.device == paNoDevice) {
        qDebug() << "Error: No default output device.\n";
        goto error;
    }
    outputParameters.channelCount = CHANNELS;
    outputParameters.device = outDevNum;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outDevNum)->defaultLowOutputLatency ;
    outputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field
    streamFlags = paNoFlag;
    err = Pa_OpenStream 	(
           &stream,
           NULL,
           &outputParameters,
           SAMPLE_RATE,
           FRAMES_PER_BUFFER,
           streamFlags,
           &processFile::paCallback,
           this
       );
    if( err != paNoError )
        goto error;
//Fill buffer on Timer to limitate Glitch
    isPaused = false;
    MainBuffer.idxNext = 0;
    fillBufferTimer = new QTimer( this );
    fillBufferTimer ->connect(fillBufferTimer, SIGNAL(timeout()),this, SLOT(insertIntoMemBuffer()));
    return (long long)0;
error:
  Pa_Terminate();
  qDebug() << "An error occured while using the portaudio stream" ;
  qDebug() << "Error number: " << err ;
  qDebug() << "Error message: " << Pa_GetErrorText( err ) ;
  return (long long)0;

}

////////////////////////////////////////////////////////////////////////////
/// \brief processFile::play
////////////////////////////////////////////////////////////////////////////
void processFile::play()
{
    PaError err;
    err = Pa_StartStream(stream);
    if( err != paNoError )
        goto error;
    isPaused = false;
    fillBufferTimer->start( 0 );
    return;
error:
  qDebug() << "An error occured while using the portaudio stream" ;
  qDebug() << "Error number: " << err ;
  qDebug() << "Error message: " << Pa_GetErrorText( err ) ;
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::pause
////////////////////////////////////////////////////////////////////////////
void processFile::pause()
{
    PaError err;
    err = Pa_StopStream(stream);
    if( err != paNoError )
        goto error;
    isPaused = true;
    return;
error:
  qDebug() << "An error occured while using the portaudio stream" ;
  qDebug() << "Error number: " << err ;
  qDebug() << "Error message: " << Pa_GetErrorText( err ) ;
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::seek
/// \param frameToStart
////////////////////////////////////////////////////////////////////////////
void processFile::seek(long long frameToStart)
{
    PaError err;

    if( !isfileOpen ){
        emit( processed((float)0));
        qDebug() << "No file is Opened";
        return;
    }


    err = Pa_IsStreamStopped(stream);
    if( err == 0){
        err = Pa_StopStream( stream );
        if( err != paNoError )
            goto error;
    }
    fillBufferTimer->stop();
    sf_seek(file,frameToStart,SEEK_SET);

    MainBuffer.isAvailable = false;
    MainBuffer.idxNext = 0;
    MainBuffer.endOfFileReached = false;

    currFileFrameRead = frameToStart;
    currPlayFrameRead = frameToStart;
    currPlayedFrameRead = frameToStart;
    listOfBlock.clear();
    if( !isPaused ){
        fillBufferTimer->start( 0 );

        err = Pa_StartStream(stream);
        if( err != paNoError )
            goto error;
    }
    return;


error:
  qDebug() << "An error occured while using the portaudio stream" ;
  qDebug() << "Error number: " << err ;
  qDebug() << "Error message: " << Pa_GetErrorText( err ) ;

}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::seek
/// \param percentOfFile
////////////////////////////////////////////////////////////////////////////
void processFile::seek(float percentOfFile )
{
    if( !isfileOpen ){
        emit( processed((float)0));
        qDebug() << "No file is Opened";
        return;
    }
    seek((long long)((float)totalFrames * percentOfFile/(float)100));
    emit( processed(percentOfFile));
}

////////////////////////////////////////////////////////////////////////////
/// \brief processFile::insertIntoMemBuffer
////////////////////////////////////////////////////////////////////////////
void processFile::insertIntoMemBuffer()
{
    long countFrameReadFile;
    long readST;
    posReadFrame pRF;

    fillBufferTimer->stop();
    qSem.acquire(1);
    while ( MainBuffer.idxNext < FRAMES_PER_BUFFER_IN_MEMORY &&  MainBuffer.endOfFileReached == false ) {
        if( bypassStrech ){
            if(pSoundTouch.numSamples() != 0 ){ // Case of Switch in middle of file
                countFrameReadFile = pSoundTouch.receiveSamples( data, FRAMES_PER_BUFFER_IN_MEMORY);
            }
            else{
                countFrameReadFile = sf_readf_float(file, data, FRAMES_PER_BUFFER_IN_MEMORY);
                if(countFrameReadFile == 0){
                    MainBuffer.endOfFileReached = true;
                    MainBuffer.isAvailable = true;
                    qSem.release(1);
                    // No more interrupt Timer, end of file
                    return;
                }
                pRF.fileFrameReadBottom = currFileFrameRead;
                pRF.playFrameReadBottom = currPlayFrameRead;
                currFileFrameRead += countFrameReadFile;
                currPlayFrameRead += countFrameReadFile;
                pRF.fileFrameReadTop= currFileFrameRead;
                pRF.playFrameReadTop = currPlayFrameRead;
                listOfBlock.append(pRF);
                garbageListOfBlock();
            }
            memcpy( &MainBuffer.buffer[MainBuffer.idxNext], data, countFrameReadFile*CHANNELS*sizeof(float));
            MainBuffer.idxNext += countFrameReadFile*CHANNELS;
        }
        else{
            if(pSoundTouch.numSamples() == 0){
                countFrameReadFile = sf_readf_float(file, data, FRAMES_PER_BUFFER_IN_MEMORY);
                if(countFrameReadFile == 0){
                    MainBuffer.endOfFileReached = true;
                    MainBuffer.isAvailable = true;
                    qSem.release(1);
                    // No more interrupt Timer, end of file
                    return;
                }
                pSoundTouch.putSamples(data,countFrameReadFile);
                pRF.fileFrameReadBottom = currFileFrameRead;
                pRF.playFrameReadBottom = currPlayFrameRead;
                currFileFrameRead += countFrameReadFile;
                currPlayFrameRead += countFrameReadFile * pSoundTouch.getInputOutputSampleRatio();
                pRF.fileFrameReadTop= currFileFrameRead;
                pRF.playFrameReadTop = currPlayFrameRead;
                listOfBlock.append(pRF);
                garbageListOfBlock();
            }
            readST = pSoundTouch.receiveSamples( data2, FRAMES_PER_BUFFER_IN_MEMORY);
            memcpy( &MainBuffer.buffer[MainBuffer.idxNext], data2, readST*CHANNELS*sizeof(float));
            MainBuffer.idxNext += readST*CHANNELS;
        }
    }
    MainBuffer.isAvailable = true;
    qSem.release(1);
    fillBufferTimer->start(TIMERTIMEOUT);
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::getMemBuffer
/// \param out
/// \param framesToRead
/// \return
////////////////////////////////////////////////////////////////////////////
long processFile::getMemBuffer(float *out,unsigned long framesToRead){
    unsigned long read;

    qSem.acquire(1);
//    elapsedTimer.start();



    if( MainBuffer.idxNext >= framesToRead*CHANNELS )
        read = framesToRead;
    else
        read =  MainBuffer.idxNext/CHANNELS;

    float prc = (float)toFrameOrig(currPlayedFrameRead)/(float)totalFrames * (float)100;
    emit( processed(prc));

    currPlayedFrameRead += read;

    memcpy(out,&MainBuffer.buffer[0],read*CHANNELS*sizeof(float));
    memcpy(&MainBuffer.buffer[0],
               &MainBuffer.buffer[read * CHANNELS],
               MainBuffer.idxNext*sizeof(float)  - read * CHANNELS*sizeof(float)
            );
    if( read < framesToRead) //End of file reached
        memset(out+read*CHANNELS,0,framesToRead*CHANNELS*sizeof(float)-MainBuffer.idxNext *sizeof(float));
    MainBuffer.idxNext -= read * CHANNELS;
    if( read == 0)
        MainBuffer.isAvailable = false;

    qSem.release(1);

    return read;
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::setTempo
/// \param tempo
////////////////////////////////////////////////////////////////////////////
void processFile::setTempo(int tempo){
//    if( currTempo != tempo ){
        pSoundTouch.setTempo((float)tempo/100);
        currTempo = tempo;

//    }
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::setPitchSemiTones
/// \param semiTone
////////////////////////////////////////////////////////////////////////////
void processFile::setPitchSemiTones(int semiTone){
//    if( currSemiTone != semiTone ){
        pSoundTouch.setPitchSemiTones(semiTone);
        currSemiTone = semiTone;
//    }
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::setOctava
/// \param octava
////////////////////////////////////////////////////////////////////////////
void processFile::setOctava(int octava){
    pSoundTouch.setPitchOctaves((float)octava);
    currOctava = octava;
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::getBPM
/// \return
////////////////////////////////////////////////////////////////////////////
float processFile::getBPM(){
    long countFrameReadFile;

    soundtouch::BPMDetect bpm(CHANNELS, SAMPLE_RATE);


    for( int i = 0; i < 30;i++){ // For first n Seconds off file
        countFrameReadFile = sf_readf_float(file, data, SAMPLE_RATE); // Put One second each time
        if( countFrameReadFile == 0)
            break;
        bpm.inputSamples(data,countFrameReadFile);
    }
    sf_seek(file,0,SEEK_SET);


    return bpm.getBpm();

}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::toFrameOrig
/// \param frameToConvert
/// \return
////////////////////////////////////////////////////////////////////////////
long long processFile::toFrameOrig(long long frameToConvert){
    long long retValue;

    for( int i = 0; i < listOfBlock.count();i++){
        if(frameToConvert >= listOfBlock.at(i).playFrameReadBottom and frameToConvert < listOfBlock.at(i).playFrameReadTop ){
            long long nbOrig = listOfBlock.at(i).fileFrameReadTop  - listOfBlock.at(i).fileFrameReadBottom;
            long long nbDest = listOfBlock.at(i).playFrameReadTop  - listOfBlock.at(i).playFrameReadBottom;
            float ratio = (float)nbOrig/(float)nbDest;
            retValue =  (long long)((float)( frameToConvert - listOfBlock.at(i).playFrameReadBottom ) * ratio + (float)listOfBlock.at(i).fileFrameReadBottom);
            frameToTime(retValue,&currTimePlayed);
            return retValue;
        }
    }
    return -1;
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::garbageListOfBlock
////////////////////////////////////////////////////////////////////////////
void processFile::garbageListOfBlock(){
    for( int i = 0; i < listOfBlock.count();i++){
        if( currPlayedFrameRead > listOfBlock.at(i).playFrameReadTop ){
            listOfBlock.removeAt(i);
            break;
        }
    }
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::frameToTime
/// \param frameToConvert
/// \param currentTime
////////////////////////////////////////////////////////////////////////////
void processFile::frameToTime(long long frameToConvert, QTime *currentTime){
    int restOfFrame = frameToConvert%SAMPLE_RATE;
    int ms = (int)((float)1000/(float)SAMPLE_RATE * (float)restOfFrame);
    int seconde = frameToConvert/SAMPLE_RATE;
    int minute = seconde/60;
    seconde = seconde%60;
    int heure = minute/60;
    minute = minute%60;
    currentTime->setHMS(heure,minute,seconde,ms);
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::getCurrentReadFrame
/// \return
////////////////////////////////////////////////////////////////////////////
long long processFile::getCurrentReadFrame(){
    return toFrameOrig(currPlayedFrameRead);
}
////////////////////////////////////////////////////////////////////////////
/// \brief processFile::getCurrentReadTime
/// \param currentTime
////////////////////////////////////////////////////////////////////////////
void processFile::getCurrentReadTime(QTime *currentTime){
    frameToTime(toFrameOrig(currPlayedFrameRead), currentTime );
}
////////////////////////////////////////////////////////////////////////////
////// \brief processFile::framesOfFile
////// \return
////////////////////////////////////////////////////////////////////////////
long long processFile::framesOfFile(){
    return totalFrames;
}
