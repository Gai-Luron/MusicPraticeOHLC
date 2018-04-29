#ifndef PROCESSFILE_H
#define PROCESSFILE_H

#include <QObject>
#include<QTimer>
#include <QString>
#include <windows.h>
#define ENABLE_SNDFILE_WINDOWS_PROTOTYPES 1
#include "sndfile.h"
#include "portaudio.h"
#include "SoundTouch.h"
#include "BPMDetect.h"
#include "STTypes.h"
#include <QTime>


class processFile : public QObject
{
    Q_OBJECT
private:
    SNDFILE *file;
    soundtouch::SoundTouch pSoundTouch;
    QTimer *fillBufferTimer;
    PaStream *stream = NULL;
    struct posReadFrame {
        long long fileFrameReadBottom;
        long long fileFrameReadTop;
        long long playFrameReadBottom;
        long long playFrameReadTop;
    };
    QList <posReadFrame> listOfBlock;
    QTime currTimePlayed;
    QString currFileName;
    long long currFileFrameRead;
    long long currPlayFrameRead;
    long long currPlayedFrameRead;
    bool isfileOpen = false;
    bool isPaused = true;



    long getMemBuffer(float *out,unsigned long framesPerBuffer);
    void garbageListOfBlock();
    long long toFrameOrig(long long frameToConvert);
    void frameToTime(long long frameToConvert, QTime *currentTime);
    long long totalFrames;


public:
    processFile();
    ~processFile();
    static int paCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *file
                           );
    long long openSoundFile(QString fileName);
    void play();
    void seek(long long frameToStart);
    void seek(float percentOfFile );
    void pause();
    void setTempo(int tempo);
    void setPitchSemiTones(int semiTone);
    void setOctava(int octava);
    float getBPM();
    long long getCurrentReadFrame();
    void getCurrentReadTime(QTime *currentTime);
    long long framesOfFile();


    bool bypassStrech = false;
    int currTempo;
    int currSemiTone;
    int currOctava;

public slots:
    void insertIntoMemBuffer();

signals:
    void processed(float num);


};

#endif // PROCESSFILE_H
