#ifndef LOOPSAUDIO_H
#define LOOPSAUDIO_H

struct loopAudio{
    bool currSelected;
    QString loopName;
    float beginLoop;
    float endLoop;
    int tempo;
    int semiTones;
};
struct configAudio {
    QString fileName;
    QString configFileName;
    QList<loopAudio> loopsAudioList;
};

#endif // LOOPSAUDIO_H

