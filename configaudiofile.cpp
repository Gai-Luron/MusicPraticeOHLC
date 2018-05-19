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


}
void configAudioFile::setFilename(QString fileName){

    bool findElement;
    currConfigAudioFile.fileName = fileName;
    QFileInfo QF;
    QF.setFile(fileName);
    QString configFileName = pathData + "/" + QF.completeBaseName() + ".xml";
    QFile xml_doc(configFileName);
retry:
    if(!xml_doc.open(QIODevice::ReadWrite))// Si l'on n'arrive pas à ouvrir le fichier XML.
    {
         QMessageBox::warning(this,tr("Erreur à l'ouverture du document XML")
                ,tr("Le document XML ") + configFileName + tr(" n'a pas pu être ouvert ou créé.")
         );
         return;
    }
    if (!dom->setContent(&xml_doc)) // Si l'on n'arrive pas à associer le fichier XML à l'objet DOM. C'est que c'est un fichier vide
    {

        xml_doc.close();
        if( createDefaultConfigFile(configFileName) )
            goto retry;
         QMessageBox::warning(this,"Erreur à l'ouverture du document XML","Le document XML n'a pas pu être attribué à l'objet QDomDocument.");
         return;
     }

retryDom:
    QDomElement domAudioFile;
    if ( getDomAudioFile(fileName,&domAudioFile ) == false){
        createAudioFileXML(fileName);
        createLoopXML(fileName,"All",0,100,0,0);
        saveDom( configFileName );
        goto retryDom;
    }

    getConfig(domAudioFile);
}
void configAudioFile::saveDom(QString configFileName)
{
    QFile data(configFileName);
    if( data.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream out(&data);
        dom->save(out,4);
        data.close();
    }
}
bool configAudioFile::getDomAudioFile( QString fileName, QDomElement *domReturn  )
{
    QDomElement domAudioFiles = dom->firstChildElement("audioFiles");
    QDomElement domElement = domAudioFiles.firstChildElement("audioFile");
    while(!domElement.isNull()){
        QDomElement name = domElement.firstChildElement("fileName");
       if( name.text() == fileName ){
           *domReturn = domElement;
           return true;
       }
       domElement = domElement.nextSiblingElement("audioFile");
    }
    return false;
}


void configAudioFile::createAudioFileXML( QString fileName )
{
    QDomElement domAudioFiles = dom->firstChildElement("audioFiles");

    QDomElement XML_AudioFile = dom->createElement("audioFile");
    domAudioFiles.appendChild(XML_AudioFile);

    QDomElement XML_fileName = dom->createElement("fileName");
    XML_AudioFile.appendChild(XML_fileName);

    QDomText XML_txt = dom->createTextNode(fileName);
    XML_fileName.appendChild(XML_txt);

    QDomElement XML_Loops = dom->createElement("loops");
    XML_AudioFile.appendChild(XML_Loops);

}

void configAudioFile::createLoopXML( QString fileName, QString name,float beginLoop, float endLoop, int tempo, int semitone )
{
    QDomElement domAudioFile;
    getDomAudioFile(fileName,&domAudioFile);


    QDomElement XML_Loops = domAudioFile.firstChildElement("loops");

    QDomElement XML_Loop = dom->createElement("loop");
    XML_Loops.appendChild(XML_Loop);


    QDomElement XML_name = dom->createElement("name");
    XML_Loop.appendChild(XML_name);
    QDomElement XML_beginLoop = dom->createElement("beginLoop");
    XML_Loop.appendChild(XML_beginLoop);
    QDomElement XML_endLoop = dom->createElement("endLoop");
    XML_Loop.appendChild(XML_endLoop);
    QDomElement XML_tempo = dom->createElement("tempo");
    XML_Loop.appendChild(XML_tempo);
    QDomElement XML_semiTones = dom->createElement("semiTones");
    XML_Loop.appendChild(XML_semiTones);

    QDomText XML_txt = dom->createTextNode(name);
    XML_name.appendChild(XML_txt);
    XML_txt = dom->createTextNode(QString::number(beginLoop));
    XML_beginLoop.appendChild(XML_txt);
    XML_txt = dom->createTextNode(QString::number(endLoop));
    XML_endLoop.appendChild(XML_txt);
    XML_txt = dom->createTextNode(QString::number(tempo));
    XML_endLoop.appendChild(XML_txt);
    XML_txt = dom->createTextNode(QString::number(semitone));
    XML_endLoop.appendChild(XML_txt);

}
bool configAudioFile::createDefaultConfigFile( QString configFileName )
{
    QDomNode node = dom->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    dom->appendChild(node);
    QDomElement XML_audioFiles = dom->createElement("audioFiles");
    dom->appendChild(XML_audioFiles);
    saveDom(configFileName);
    return true;

}
void configAudioFile::getConfig(QDomElement domElement )
{

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
