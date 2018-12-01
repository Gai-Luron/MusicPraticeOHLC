#include "processfile.h"

processFile::processFile()
{
    soundTouch = new frontSoundTouch();
    soundTouch->connect(soundTouch, SIGNAL(processed(double)),this, SLOT(receiveProcessed(double)));
    caf = new configAudioFile();
}
processFile::~processFile()
{
    delete soundTouch;
    delete caf;
}
void processFile::setFilename(QString fileName){
    this->fileName = fileName;
    soundTouch->setFilename(fileName);
    this->loadConfig();
    loopsAudioList = &caf->loopsAudioList;
    caf->setSelected(0);
}
bool processFile::play(){
    return soundTouch->play();
}
void processFile::seek(long long frameToStart){
    soundTouch->seek(frameToStart);
}
void processFile::seek(double percentOfFile ){
    soundTouch->seek(percentOfFile);
}
void processFile::pause(){
    soundTouch->pause();
}
void processFile::setTempo(int tempo){
    soundTouch->setTempo(tempo);
}
void processFile::setPitchSemiTones(int semiTone){
    soundTouch->setPitchSemiTones(semiTone);
}
void processFile::setOctava(int octava){
    soundTouch->setOctava(octava);
}
float processFile::getBPM(){
    return soundTouch->getBPM();
}
long long processFile::getCurrentReadFrame(){
    return soundTouch->getCurrentReadFrame();
}
void processFile::getCurrentReadTime(QTime *currentTime){
    soundTouch->getCurrentReadTime(currentTime);
}
long long processFile::framesOfFile(){
    return soundTouch->framesOfFile();
}
void processFile::setByPassStrech(bool flag){
    soundTouch->bypassStrech = flag;
}
bool processFile::getByPassStrech(){
    return soundTouch->bypassStrech;
}

void processFile::receiveProcessed(double perc){
   emit( processed(perc));
}
void processFile::setSelectedLoop(int i ){
    caf->setSelected(i);
}
int processFile::selectedLoop(){
    return caf->selected();
}
void processFile::loadConfig(){
    caf->loadConfig(this->fileName);
}
void processFile::saveConfig(){
    caf->saveConfig();
}


