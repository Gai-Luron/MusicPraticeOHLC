#ifndef LOOPSAUDIO_H
#define LOOPSAUDIO_H
#define MAX_LOOPS 100

struct loopAudio{
    QString loopName;
    float beginLoop;
    float endLoop;
    int tempo;
    int semiTones;
};
struct configAudio {
    QString fileName;
    int nbLoops;
    loopAudio loopsAudio[MAX_LOOPS];
};

#endif // LOOPSAUDIO_H

