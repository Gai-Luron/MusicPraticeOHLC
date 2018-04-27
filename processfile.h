#ifndef PROCESSFILE_H
#define PROCESSFILE_H

#include <QObject>
#include<QTimer>
#include <QString>
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
    long long currFileFrameRead;
    long long currPlayFrameRead;
    long long currPlayedFrameRead;


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
    void pause();
    void setTempo(int tempo);
    float getBPM();
    long long getCurrentReadFrame();
    void getCurrentReadTime(QTime *currentTime);
    long long framesOfFile();


    bool bypassStrech = false;
    int currTempo;

public slots:
    void insertIntoMemBuffer();

signals:
    void processed(int num);


};

#endif // PROCESSFILE_H
