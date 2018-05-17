#include "configaudiofile.h"
#include <QMessageBox>
#include <QWidget>


configAudioFile::configAudioFile()
{
    QStringList listeOfStdPaths;


    listeOfStdPaths = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    pathData = listeOfStdPaths.at(0);

    QDir dir(pathData);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    dom = new QDomDocument("CONFIG");

    QString fileName = pathData + "/" + "configAudioFile.xml";
    QFile xml_doc(fileName);
    if(!xml_doc.open(QIODevice::ReadOnly))// Si l'on n'arrive pas à ouvrir le fichier XML.
    {
         QMessageBox::warning(this,"Erreur à l'ouverture du document XML"
                                    ,"Le document XML n'a pas pu être ouvert. Vérifiez que le nom est le bon et que le document est bien placé"
                              );
         return;
    }
    if (!dom->setContent(&xml_doc)) // Si l'on n'arrive pas à associer le fichier XML à l'objet DOM.
    {
         xml_doc.close();
         QMessageBox::warning(this,"Erreur à l'ouverture du document XML","Le document XML n'a pas pu être attribué à l'objet QDomDocument.");
         return;
     }
    xml_doc.close();

}
void configAudioFile::setFilename(QString fileName){

    currConfigAudioFile.fileName = fileName;


    QDomElement domElement = dom->firstChildElement("audioFiles");
    domElement = domElement.firstChildElement("audioFile");
    while(!domElement.isNull()){
       QDomElement name = domElement.firstChildElement("fileName");
       if( name.text() == fileName ){
           getConfig(domElement);
       }
//       qDebug() << "Nom du fichier" << name.text();
       domElement = domElement.nextSiblingElement("audioFile");
    }
}
void configAudioFile::getConfig(QDomElement domElement ){

    QDomElement name = domElement.firstChildElement("fileName");
    QDomElement domLoop = domElement.firstChildElement("loops");
    domLoop = domLoop.firstChildElement("loop");
    currConfigAudioFile.loopsAudioList.clear();
    while(!domLoop.isNull()){
        loopAudio tmpLoopAudio;
        tmpLoopAudio.loopName = domLoop.firstChildElement("name").text();
        tmpLoopAudio.beginLoop = domLoop.firstChildElement("beginLoop").text().toFloat();
        tmpLoopAudio.endLoop = domLoop.firstChildElement("endLoop").text().toFloat();
        tmpLoopAudio.tempo = domLoop.firstChildElement("tempo").text().toFloat();
        tmpLoopAudio.semiTones = domLoop.firstChildElement("semiTones").text().toInt();
        currConfigAudioFile.loopsAudioList.append(tmpLoopAudio);

        domLoop = domLoop.nextSiblingElement("loop");
    }
}
/*
void configAudioFile::getTempo(QString loopName ){
    for(int i = 0; i < MAX_LOOPS;i++){

    }
}
*/
