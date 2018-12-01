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
public:
    QList<configAudioFile::loopAudio> *loopsAudioList;


    processFile();
    ~processFile();

    void setFilename(QString fileName);
    bool play();
    void seek(long long frameToStart);
    void seek(double percentOfFile );
    void pause();
    void setByPassStrech(bool flag);
    bool getByPassStrech();
    void setTempo(int tempo);
    void setPitchSemiTones(int semiTone);
    void setOctava(int octava);
    float getBPM();
    long long getCurrentReadFrame();
    void getCurrentReadTime(QTime *currentTime);
    long long framesOfFile();
    void setSelectedLoop(int i );
    int selectedLoop();
    void loadConfig();
    void saveConfig();




private slots:
    void receiveProcessed(double num);

signals:
    void processed(double num);


};

#endif // PROCESSFILE_H
