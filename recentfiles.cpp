#include "recentfiles.h"
#include <QStandardPaths>
#include <QDebug>
#include <QDir>
#include <QFile>

#define MAXFILE 50

recentFiles::recentFiles()
{
    QStringList listeOfStdPaths;


    listeOfStdPaths = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    pathData = listeOfStdPaths.at(0);

    QDir dir(pathData);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    load();

    return;
}
QStringList recentFiles::listOfFiles()
{
    return lof;
}
void recentFiles::addFile(QString fileName){
    lof.removeOne(fileName);
    lof.prepend(fileName);
    if( lof.size() > MAXFILE )
        lof.removeLast();
    save();
    return;
}
void recentFiles::load(){
    QString fileName;
    QString lineRead;

    lof.clear();
    fileName = pathData + "/" + "RecentsFiles.txt";
    QFile handleFile(fileName);
    if( handleFile.open( QIODevice::ReadOnly | QIODevice::Text) ){
        QTextStream stream(&handleFile);
        stream.setCodec("UTF-8");
        while(true){

            if( stream.atEnd())
                break;
            lineRead = stream.readLine(300);
            qDebug() << lineRead;
            if( lineRead != "" ){
                lof.append( lineRead );
            }
        }
    }
    return;
}

void recentFiles::save(){
    QString fileName;

    fileName = pathData + "/" + "RecentsFiles.txt";
    QFile handleFile(fileName);
    if( handleFile.open( QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) ){
        QTextStream stream(&handleFile);
        stream.setCodec("UTF-8");
        for(int i = 0; i < lof.size();i++){
            stream << lof.at(i) << endl;
        }
    handleFile.close();
    }
}
