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



class configAudioFile : public QWidget
{
public:
    struct loopAudio{
        bool currSelected;
        QString loopName;
        double beginLoop;
        double endLoop;
        int tempo;
        int semiTones;
    };
    configAudioFile();
    ~configAudioFile();
    void loadConfig(QString fileName);
    void saveConfig();
    void setSelected(int i );
    int selected();

    QString fileName;
    QString configFileName;
    QList<loopAudio> loopsAudioList;


private:

    void saveDom();
    bool getDomAudioFile( QString fileName, QDomElement* domReturn );
    bool createDefaultConfigFile( QString configFilename );
    void getQDomAudioFile( QString fileName );
    void createLoopXML( QString fileName, QString name,double beginLoop, double endLoop, int tempo, int semitone );
    void createAudioFileXML( QString fileName );



    QString pathData;
    QDomDocument *dom;


};

#endif // CONFIGAUDIOFILE_H
