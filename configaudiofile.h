#ifndef CONFIGAUDIOFILE_H
#define CONFIGAUDIOFILE_H

#include <QString>
#include <QtXml>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QtGui>
#include <QWidget>
#include "loopsaudio.h"



class configAudioFile : public QWidget
{

public:
    configAudioFile();
    void setFilename(QString fileName);
    configAudio currConfigAudioFile;

private:
    void getConfig(QDomElement domElement );

    QString pathData;
    QDomDocument *dom;


};

#endif // CONFIGAUDIOFILE_H
