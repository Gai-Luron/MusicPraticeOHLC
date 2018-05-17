#ifndef LOOPSAUDIO_H
#define LOOPSAUDIO_H

struct loopAudio{
    QString loopName;
    float beginLoop;
    float endLoop;
    int tempo;
    int semiTones;
};
struct configAudio {
    QString fileName;
    QList<loopAudio> loopsAudioList;
};

#endif // LOOPSAUDIO_H

