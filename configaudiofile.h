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
    void saveDom(QString configFileName);
    bool getDomAudioFile( QString fileName, QDomElement* domReturn );
    bool createDefaultConfigFile( QString configFilename );
    void getQDomAudioFile( QString fileName );
    void createLoopXML( QString fileName, QString name,float beginLoop, float endLoop, int tempo, int semitone );
    void createAudioFileXML( QString fileName );



    QString pathData;
    QDomDocument *dom;


};

#endif // CONFIGAUDIOFILE_H
