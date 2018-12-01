#ifndef PROCESSFILE_H
#define PROCESSFILE_H

#include <QObject>
#include "frontSoundTouch.h"
#include "configaudiofile.h"

class processFile: public QObject
{
     Q_OBJECT
private:
    QString fileName;
    frontSoundTouch * soundTouch;
    configAudioFile * caf;
    QList<loopAudio> *loopsAudioList;
    int iterationLoop;
    bool alreadyLooped = false;

public:

    processFile();
    ~processFile();

    void setFilename(QString fileName);
    bool play();
    void seek(long long frameToStart);
    void seek(double percentOfFile );
    void pause();
    void setByPassStrech(bool flag);
    bool getByPassStrech();
    void setTempoDirect(int tempo);
    void setPitchSemiTonesDirect(int semiTone);
    void setOctavaDirect(int octava);
    float getBPM();
    long long getCurrentReadFrame();
    void getCurrentReadTime(QTime *currentTime);
    long long framesOfFile();
    void appendLoop(loopAudio tmpLoopAudio);
    void insertLoop(loopAudio tmpLoopAudio, int idx = -1 );
    void removeLoop( int idx = -1 );
    void moveLoop( int idxOrig, int idxDest );

    void setLoopName( QString newName, int idx = -1 );
    QString loopName( int idx = -1 );

    int countLoop();

    int currentLoop();
    void setCurrentLoop( int i );

    double beginLoop( int idx = -1 );
    void setBeginLoop( double perc ,int idx = -1  );

    double endLoop( int idx = -1 );
    void setEndLoop( double perc,int idx = -1 );

    int semiTones( int idx = -1);
    void setSemiTones( int val,int idx = -1 );

    int tempo(int idx = -1);
    void setTempo( int val,int idx = -1 );

    int nbLoop( int idx =-1 );
    void setNbLoop( int val, int idx = - 1 );

    void loadConfig();
    void saveConfig();




private slots:
    void receiveProcessed(double num);

signals:
    void processed(double num);
    void requestPause();
    void loopRemaining(int nbLoop);


};

#endif // PROCESSFILE_H
