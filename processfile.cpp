#include "processfile.h"

processFile::processFile()
{
    soundTouch = new frontSoundTouch();
    soundTouch->connect(soundTouch, SIGNAL(processed(double)),this, SLOT(receiveProcessed(double)));
    caf = new configAudioFile();
}
processFile::~processFile()
{
    delete soundTouch;
    delete caf;
}
void processFile::setFilename(QString fileName){
    this->fileName = fileName;
    soundTouch->setFilename(fileName);
    this->loadConfig();
    loopsAudioList = &caf->loopsAudioList;
    setCurrentLoop(0);
}
bool processFile::play(){
    qDebug() << "Play";
    return soundTouch->play();
}
void processFile::pause(){
    qDebug() << "Pause";
    soundTouch->pause();
}
void processFile::seek(long long frameToStart){
    soundTouch->seek(frameToStart);
}
void processFile::seek(double percentOfFile ){
    soundTouch->seek(percentOfFile);
}
void processFile::setTempoDirect(int tempo){
    soundTouch->setTempo(tempo);
}
void processFile::setPitchSemiTonesDirect(int semiTone){
    soundTouch->setPitchSemiTones(semiTone);
}
void processFile::setOctavaDirect(int octava){
    soundTouch->setOctava(octava);
}
float processFile::getBPM(){
    return soundTouch->getBPM();
}
long long processFile::getCurrentReadFrame(){
    return soundTouch->getCurrentReadFrame();
}
void processFile::getCurrentReadTime(QTime *currentTime){
    soundTouch->getCurrentReadTime(currentTime);
}
long long processFile::framesOfFile(){
    return soundTouch->framesOfFile();
}
void processFile::setByPassStrech(bool flag){
    soundTouch->bypassStrech = flag;
}
bool processFile::getByPassStrech(){
    return soundTouch->bypassStrech;
}

void processFile::receiveProcessed(double perc){
    int currSelected;

    currSelected = currentLoop();
    if( perc >= loopsAudioList->at(currSelected).endLoop
            && ( perc  <= loopsAudioList->at(currSelected).endLoop + 0.2 || perc >= 100)
            && alreadyLooped == false
     ){
        perc = loopsAudioList->at(currSelected).beginLoop;
        seek(double(perc));
        if( iterationLoop > 0  ){
            iterationLoop--;
            emit( loopRemaining(iterationLoop));
        }
        if( iterationLoop == 0){
            emit( requestPause());
            iterationLoop = nbLoop();
            emit( loopRemaining(iterationLoop));
        }
        alreadyLooped = true;
    }
    else{
        alreadyLooped = false;
    }

    emit( processed(perc));
}
void processFile::setCurrentLoop(int i ){
    caf->setSelected(i);
    iterationLoop = nbLoop(i);
}
int processFile::currentLoop(){
    return caf->selected();
}
void processFile::appendLoop(loopAudio tmpLoopAudio){
    loopsAudioList->append(tmpLoopAudio);
    setCurrentLoop(loopsAudioList->count()-1);
}
void processFile::insertLoop(loopAudio tmpLoopAudio, int idx ){
    if( idx == -1 )
        idx = currentLoop();
    loopsAudioList->insert(idx,tmpLoopAudio);
    setCurrentLoop( idx );
}
void processFile::removeLoop( int idx ){
    if( idx == -1 )
        idx = currentLoop();
    loopsAudioList->removeAt(idx);
    setCurrentLoop(idx-1);
}
void processFile::setLoopName( QString newName, int idx  ){
    if( idx == -1 )
        idx = currentLoop();
    (*loopsAudioList)[idx].loopName = newName;

}
QString processFile::loopName( int idx ){
    if( idx == -1 )
        idx = currentLoop();
    return loopsAudioList->at(idx).loopName;

}
int processFile::countLoop(){
   return loopsAudioList->count();
}

void processFile::moveLoop( int idxOrig, int idxDest ){
    loopsAudioList->move(idxOrig,idxDest);
}

double processFile::beginLoop( int idx ){
    if( idx == -1 )
        idx = currentLoop();
    return loopsAudioList->at(idx).beginLoop;
}

void processFile::setBeginLoop( double perc, int idx ){
    if( idx == -1)
        idx = currentLoop();

    if( perc < 0 )
        perc = 0;
    if( perc > 100 )
        perc = 100;

    (*loopsAudioList)[idx].beginLoop = perc;

}

double processFile::endLoop( int idx ){
    if( idx == -1)
        idx = currentLoop();

    return loopsAudioList->at(idx).endLoop;
}

void processFile::setEndLoop( double perc, int idx ){
    if( idx == -1)
        idx = currentLoop();

    if( perc < 0 )
        perc = 0;
    if( perc > 100 )
        perc = 100;

    (*loopsAudioList)[idx].endLoop = perc;
}


int processFile::semiTones( int idx ){
    if( idx == -1)
        idx = currentLoop();

    return loopsAudioList->at(idx).semiTones;
}

void processFile::setSemiTones( int val, int idx ){
    if( idx == -1)
        idx = currentLoop();

    (*loopsAudioList)[idx].semiTones = val;
    soundTouch->setPitchSemiTones(val);

}



int processFile::tempo( int idx ){
    if( idx == -1)
        idx = currentLoop();
    return loopsAudioList->at(idx).tempo;
}

void processFile::setTempo( int val, int idx ){
    if( idx == -1)
        idx = currentLoop();

    (*loopsAudioList)[idx].tempo = val;
    soundTouch->setTempo(val);

}
int processFile::nbLoop( int idx ){
    if( idx == -1)
        idx = currentLoop();
    return loopsAudioList->at(idx).nbLoop;
}

void processFile::setNbLoop( int val, int idx ){
    if( idx == -1)
        idx = currentLoop();

    (*loopsAudioList)[idx].nbLoop = val;

}

void processFile::loadConfig(){
    caf->loadConfig(this->fileName);
}
void processFile::saveConfig(){
    caf->saveConfig();
}




