#include "configaudiofile.h"
#include <QMessageBox>
#include <QWidget>

loopAudio::loopAudio()
{
    loopName = "Nouveau";
    currSelected = false;
    beginLoop = 0;
    endLoop = 100;
    tempo = 100;
    semiTones = 0;
    nbLoop = -1;


}
loopAudio::~loopAudio()
{
}

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
configAudioFile::~configAudioFile()
{
    delete dom;
}
void configAudioFile::saveConfig()
{
    QDomElement domAudioFile;
    getDomAudioFile(fileName,&domAudioFile );
    QDomElement XML_loops = domAudioFile.firstChildElement("loops");
    XML_loops.parentNode().removeChild(XML_loops);
    XML_loops = dom->createElement("loops");
    domAudioFile.appendChild(XML_loops);



    for( int i = 0; i < loopsAudioList.count();i++){

        createLoopXML(fileName,loopsAudioList.at(i) );
    }


    saveDom( );

}

void configAudioFile::loadConfig(QString fileName){

    this->fileName = fileName;
    QFileInfo QF;
    QF.setFile(fileName);
    configFileName = pathData + "/" + QF.completeBaseName() + ".xml";

    QFile xml_doc(configFileName);
//    qDebug() << configFileName;

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
        loopAudio lA;
        lA.loopName = "All";
        lA.beginLoop = 0;
        lA.endLoop = 100;
        lA.tempo = 100;
        lA.semiTones = 0;
        lA.nbLoop = 1;
        createLoopXML(fileName,lA);
        saveDom( );
        goto retryDom;
    }

    QDomElement name = domAudioFile.firstChildElement("fileName");
    QDomElement domLoop = domAudioFile.firstChildElement("loops");
    domLoop = domLoop.firstChildElement("loop");
    loopsAudioList.clear();
    while(!domLoop.isNull()){
        loopAudio tmpLoopAudio;
        tmpLoopAudio.loopName = domLoop.firstChildElement("name").text();
        tmpLoopAudio.beginLoop = domLoop.firstChildElement("beginLoop").text().toDouble();
        tmpLoopAudio.endLoop = domLoop.firstChildElement("endLoop").text().toDouble();
        tmpLoopAudio.tempo = domLoop.firstChildElement("tempo").text().toInt();
        tmpLoopAudio.semiTones = domLoop.firstChildElement("semiTones").text().toInt();
        tmpLoopAudio.nbLoop = domLoop.firstChildElement("nbLoop").text().toInt();
        if (tmpLoopAudio.beginLoop > tmpLoopAudio.endLoop){
            tmpLoopAudio.beginLoop = 0;
            tmpLoopAudio.endLoop = 100;
        }

        loopsAudioList.append(tmpLoopAudio);

        domLoop = domLoop.nextSiblingElement("loop");
    }
    setSelected(0);

}

void configAudioFile::saveDom()
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

void configAudioFile::createLoopXML( QString fileName, loopAudio lA )
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

    QDomElement XML_nbLoop = dom->createElement("nbLoop");
    XML_Loop.appendChild(XML_nbLoop);

    QDomText XML_txt = dom->createTextNode(lA.loopName);
    XML_name.appendChild(XML_txt);

    XML_txt = dom->createTextNode(QString::number(lA.beginLoop));
    XML_beginLoop.appendChild(XML_txt);

    XML_txt = dom->createTextNode(QString::number(lA.endLoop));
    XML_endLoop.appendChild(XML_txt);

    XML_txt = dom->createTextNode(QString::number(lA.tempo));
    XML_tempo.appendChild(XML_txt);

    XML_txt = dom->createTextNode(QString::number(lA.semiTones));
    XML_semiTones.appendChild(XML_txt);

    XML_txt = dom->createTextNode(QString::number(lA.nbLoop));
    XML_nbLoop.appendChild(XML_txt);

}
bool configAudioFile::createDefaultConfigFile( QString configFileName )
{
    QDomNode node = dom->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    dom->appendChild(node);
    QDomElement XML_audioFiles = dom->createElement("audioFiles");
    dom->appendChild(XML_audioFiles);
    saveDom();
    return true;

}
void configAudioFile::setSelected(int idx )
{
    for( int i = 0; i < loopsAudioList.count();i++){
        if( idx == i )
            loopsAudioList[i].currSelected = true;
        else
            loopsAudioList[i].currSelected = false;
    }
}
int configAudioFile::selected()
{
    for( int i = 0; i < loopsAudioList.count();i++){
        if( loopsAudioList[i].currSelected == true )
            return i;
    }
    return -1;
}
