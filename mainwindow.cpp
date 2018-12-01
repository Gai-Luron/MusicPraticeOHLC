#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configaudiofile.h"
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QPushButton>
#include <QMenu>
#include <QMessageBox>







MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->currentFileRead->setText(tr("Pas de fichier"));
    ui->currentPathRead->setText("");
    pFileObj =new processFile();
    pFileObj->setByPassStrech(false);
    connect(pFileObj, SIGNAL(processed(double)),this, SLOT(setCurrentTimePlayed(double)));

    ui->valueTempo->setText(QString::number(ui->sliderTempo->value()));
    ui->valueSemiTone->setText(QString::number(ui->sliderPitch->value()));
    ui->recentFilesWidget->addAllowedExtension("wav");
    ui->recentFilesWidget->addAllowedExtension("ogg");
    ui->recentFilesWidget->addAllowedExtension("flac");
    connect(ui->recentFilesWidget,SIGNAL(filesListDropped(QStringList*)),this,SLOT(filesDropped(QStringList*)));
    updateRecentFilesWidget();
    delayedSetButtonLoop = new QTimer( this );
    delayedSetButtonLoop->connect(delayedSetButtonLoop, SIGNAL(timeout()),this, SLOT(doDelayedSetButtonLoop()));
    connect( ui->frameMark, SIGNAL(percChanged(double,QString)), this, SLOT(slotPercChanged(double,QString)) );


}
MainWindow::~MainWindow()
{
    delete ui;
    delete delayedSetButtonLoop;
    delete pFileObj;
}
void MainWindow::keyPressEvent(QKeyEvent *event)
 {
    if(event->key() == Qt::Key_Space){
         qDebug() << "IsChecked : " << ui->actionPlay->isChecked();
        if ( ui->actionPlay->isChecked() ){
            ui->actionPlay->setChecked(false);
        }
        else{
             ui->actionPlay->setChecked(true);
        }
        this->on_actionPlay_triggered();
    }
 }
void MainWindow::slotPercChanged(double newPerc, QString markName){
    int currSelected;
    currSelected = pFileObj->selectedLoop();

    if( currSelected != 0 ){ // Not The All Loop
        if( markName == "Begin"){
            if( newPerc < (*pFileObj->loopsAudioList)[currSelected].endLoop)
                 (*pFileObj->loopsAudioList)[currSelected].beginLoop = newPerc;
            else
                ui->frameMark->updatePosMark("Begin",(*pFileObj->loopsAudioList)[currSelected].beginLoop);
        }
        if( markName == "End"){
            if( newPerc > (*pFileObj->loopsAudioList)[currSelected].beginLoop)
                (*pFileObj->loopsAudioList)[currSelected].endLoop = newPerc;
            else
                ui->frameMark->updatePosMark("End",(*pFileObj->loopsAudioList)[currSelected].endLoop);

        }
        pFileObj->saveConfig();
    }
    else{
        ui->frameMark->updatePosMark("Begin",(*pFileObj->loopsAudioList)[currSelected].beginLoop);
        ui->frameMark->updatePosMark("End",(*pFileObj->loopsAudioList)[currSelected].endLoop);
    }

}
void MainWindow::triggerloopContextMenu(loopContextMenu lc,int idx)
{
    QString action;
    configAudioFile::loopAudio tmpLoopAudio;
    switch( lc )
    {
        case loopContextMenu::ajouter:
            action = "Ajouter";
            tmpLoopAudio.loopName = tr("Nouveau");
            tmpLoopAudio.beginLoop = 0;
            tmpLoopAudio.endLoop = 100;
            tmpLoopAudio.tempo = 100;
            tmpLoopAudio.semiTones = 0;
            pFileObj->loopsAudioList->append(tmpLoopAudio);
            pFileObj->setSelectedLoop(pFileObj->loopsAudioList->count()-1);
            delayedSetButtonLoop->start(10);
            break;
        case loopContextMenu::inserer:
            if(idx > 0){
                action = "Insérer";
                tmpLoopAudio.loopName = tr("Nouveau");
                tmpLoopAudio.beginLoop = 0;
                tmpLoopAudio.endLoop = 100;
                tmpLoopAudio.tempo = 100;
                tmpLoopAudio.semiTones = 0;
                pFileObj->loopsAudioList->insert(idx,tmpLoopAudio);
                pFileObj->setSelectedLoop( idx );
                delayedSetButtonLoop->start(10);
            }
            else if (idx == 0){
                QMessageBox msgBox;
                msgBox.setText(tr("Impossible d'insérer devant la boucle principale"));
                msgBox.exec();
            }
            break;
        case loopContextMenu::supprimer:
            if( idx > 0 ){
                pFileObj->loopsAudioList->removeAt(idx);
                pFileObj->setSelectedLoop(idx-1);
                delayedSetButtonLoop->start(10);
            }
            else if(idx == 0){
                QMessageBox msgBox;
                msgBox.setText(tr("Impossible de supprimer la boucle principale"));
                msgBox.exec();
            }
            break;
    }
}

void MainWindow::filesDropped(QStringList* fileList ){
    for(int i = fileList->count()-1; i >= 0; i--){
        rFiles->addFile(fileList->at(i));
    }
    startNewaudioFile(fileList->at(0),true);
}


void MainWindow::editedNameLoop( int i, QString txt )
{
    (*pFileObj->loopsAudioList)[i].loopName = txt;
    pFileObj->saveConfig();
    pFileObj->setSelectedLoop( i );
}

void MainWindow::droppedLoopButOnOtherLoopBut( int orig, int dest)
    {

    pFileObj->loopsAudioList->move(orig,dest);
    delayedSetButtonLoop->start(10);

}
void MainWindow::doDelayedSetButtonLoop(){
    delayedSetButtonLoop->stop();
    setButtonLoops();
    pFileObj->saveConfig();
}
void MainWindow::setButtonLoops(){
    QPushButtonLoop *but;

    deleteLayout(ui->layoutLoopAll);
    deleteLayout( ui->gridLayoutLoops);


    for( int i = 0; i < pFileObj->loopsAudioList->count();i++){
        but = new QPushButtonLoop(this);
        but->setObjectName("Bloop" + QString::number(i));
        but->setProperty("myId",i);
        but->setText(pFileObj->loopsAudioList->at(i).loopName);
        but->setCheckable(true);
        but->setAutoExclusive(true);
        if( i == 0){
            ui->layoutLoopAll->addWidget(but);
        }
        else{
            ui->gridLayoutLoops->addWidget(but,(i-1)/4,(i-1)%4);
        }
        but->setChecked(pFileObj->loopsAudioList->at(i).currSelected);
        connect(but, SIGNAL(contextMenuAction(loopContextMenu,int)),this,SLOT(triggerloopContextMenu(loopContextMenu,int)));
        connect(but,SIGNAL(clicked()), this,SLOT(onButtonLoopClicked()));
        connect(but, SIGNAL(changePosButtonLoop(int,int)),this,SLOT(droppedLoopButOnOtherLoopBut(int,int)));
        connect(but, SIGNAL(editedNameLoop(int,QString)), this,SLOT(editedNameLoop(int,QString)) );
    }
}
void MainWindow::deleteLayout(QLayout *item){
    while (item->count() > 0) {
        QLayoutItem *child = item->takeAt(0);
        if (child == nullptr) continue;
        if (child->layout() != nullptr) deleteLayout(child->layout());
        if (child->widget()) delete child->widget();
        delete child;
    }
}
void MainWindow::onButtonLoopClicked(){
    QVariant myId = sender()->property("myId");
     if (myId.isValid()) {
       int idx = myId.toInt();
       pFileObj->setSelectedLoop(idx);
       setPlayConfigFromCurrentLoop();
     }

}
void MainWindow::updateRecentFilesWidget(){
    QStringList headers;
    if( rFiles )
            delete rFiles; // Free Heap of old pointer
    rFiles = new recentFiles();
    ui->recentFilesWidget->clear();
    ui->recentFilesWidget->setHeaderLabel("Derniers fichiers Audios");
    ui->recentFilesWidget->setColumnCount(4);
    ui->recentFilesWidget->hideColumn(3);
    ui->recentFilesWidget->setColumnWidth(0,200);
    ui->recentFilesWidget->setColumnWidth(1,50);
    ui->recentFilesWidget->setColumnWidth(2,300);
    headers << "Fichier" << "Ext" << "Chemin";
    ui->recentFilesWidget->setHeaderLabels(headers);



    QFileInfo QF;
    QTreeWidgetItem *itemTree;
    for( int i = 0; i < rFiles->listOfFiles().count(); i++){
        QF.setFile(rFiles->listOfFiles().at(i));
        itemTree = new QTreeWidgetItem();
        itemTree->setText(0, QF.completeBaseName());
        itemTree->setText(1, QF.suffix());
        itemTree->setText(2, QF.path());
        itemTree->setText(3, rFiles->listOfFiles().at(i));
        ui->recentFilesWidget->addTopLevelItem(itemTree);
        if( i == 0)
            ui->recentFilesWidget->setCurrentItem(itemTree);
    }
}
void MainWindow::setCurrentTimePlayed(double nb)
{
    int currSelected;
    currSelected = pFileObj->selectedLoop();

    if( flagUpdateSliderTimePlayed ){
        if( nb >= pFileObj->loopsAudioList->at(currSelected).endLoop
                && nb <= pFileObj->loopsAudioList->at(currSelected).endLoop + 0.2
         ){
            nb = pFileObj->loopsAudioList->at(currSelected).beginLoop;
            pFileObj->seek(double(nb));
        }
        ui->currentTimePlayed->setValue(int( nb*100 ));
    }
}



void MainWindow::on_checkByPass_stateChanged(int arg1)
{
    if( arg1 == Qt::Unchecked)
        pFileObj->setByPassStrech(true);
    else
        pFileObj->setByPassStrech(false);

}



void MainWindow::on_currentTimePlayed_sliderPressed()
{
    flagUpdateSliderTimePlayed = false;
}

void MainWindow::on_currentTimePlayed_sliderReleased()
{
    flagUpdateSliderTimePlayed = true;
    pFileObj->seek( ui->currentTimePlayed->value()/100.0);

}

void MainWindow::on_valueTempo_editingFinished()
{
    if( ui->valueTempo->text().toInt() < ui->sliderTempo->minimum() ){
        ui->valueTempo->setText(QString::number(ui->sliderTempo->minimum()));
    }
    if( ui->valueTempo->text().toInt() > ui->sliderTempo->maximum() ){
        ui->valueTempo->setText(QString::number(ui->sliderTempo->maximum()));
    }
    ui->sliderTempo->setValue(ui->valueTempo->text().toInt());
    pFileObj->setTempo(ui->sliderTempo->value());

}

void MainWindow::on_sliderPitch_valueChanged(int value)
{
    int currSelected;
    currSelected = pFileObj->selectedLoop();
    (*pFileObj->loopsAudioList)[currSelected].semiTones = value;
    pFileObj->saveConfig();

    ui->valueSemiTone->setText(QString::number(value));
    pFileObj->setPitchSemiTones(value);
}

void MainWindow::on_sliderTempo_valueChanged(int value)
{
    int currSelected;
    currSelected = pFileObj->selectedLoop();
    (*pFileObj->loopsAudioList)[currSelected].tempo = value;
    pFileObj->saveConfig();

    ui->valueTempo->setText(QString::number(value));
    pFileObj->setTempo(value);
}


void MainWindow::on_valueSemiTone_editingFinished()
{
    if( ui->valueSemiTone->text().toInt() < ui->sliderPitch->minimum() )
        ui->valueSemiTone->setText(QString::number(ui->sliderPitch->minimum()));
    if( ui->valueSemiTone->text().toInt() > ui->sliderPitch->maximum() )
        ui->valueSemiTone->setText(QString::number(ui->sliderPitch->maximum()));
    ui->sliderPitch->setValue(ui->valueSemiTone->text().toInt());
    pFileObj->setPitchSemiTones(ui->sliderPitch->value());


}
void MainWindow::startNewaudioFile( QString fileName, bool launchPlay ){
    rFiles->addFile(fileName);
    updateRecentFilesWidget();
    pFileObj->setFilename(fileName);
    setButtonLoops();

    setPlayConfigFromCurrentLoop();

    if( launchPlay ){
        pFileObj->play();
        ui->actionPlay->setChecked(true);
    }

}
void MainWindow::on_recentFilesWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    QString fileName;
    (void)column;
    ui->currentFileRead->setText(item->text(0));
    ui->currentPathRead->setText(item->text(2));
    fileName = item->text(3);
    startNewaudioFile(fileName, true);
}

void MainWindow::on_openFile_triggered()
{
    QString fileName;

    fileName = QFileDialog::getOpenFileName(this,
        tr("Ouvrir Fichier Audio"), ".", tr("Fichier Audio (*.wav *.ogg *.flac )"));
    if( fileName != "")
        startNewaudioFile(fileName, true);

}

void MainWindow::on_actionPlay_triggered()
{
    if ( ui->actionPlay->isChecked()){
        qDebug() << "play" << ui->actionPlay->isChecked();
        if ( !pFileObj->play() ){
            ui->actionPlay->setChecked(false);
        }
    }
    else{
        qDebug() << "Pause";
        pFileObj->pause();
    }
}

void MainWindow::on_actionPause_triggered()
{
    pFileObj->pause();
    ui->actionPlay->setChecked(false);
}

void MainWindow::on_recentFilesWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    QString fileName;
    static bool flagFirst = true;
    (void)previous;
    if( flagFirst ){
        flagFirst = false;
        ui->currentFileRead->setText(current->text(0));
        ui->currentPathRead->setText(current->text(2));
        fileName = current->text(3);
        pFileObj->setFilename(fileName);
        setButtonLoops();
        setPlayConfigFromCurrentLoop();

    }

}

void MainWindow::onContextMenuNewLoopClicked()
{
    triggerloopContextMenu(loopContextMenu::ajouter,0);
}

void MainWindow::onContextMenuDeleteLoopClicked()
{
    triggerloopContextMenu(loopContextMenu::supprimer,pFileObj->selectedLoop());
}

void MainWindow::onContextMenuInsertLoopClicked()
{
    triggerloopContextMenu(loopContextMenu::inserer,pFileObj->selectedLoop());
}
void MainWindow::setPlayConfigFromCurrentLoop(){
    int currSelected;
    currSelected = pFileObj->selectedLoop();
    ui->sliderTempo->setValue(pFileObj->loopsAudioList->at(currSelected).tempo);
    ui->sliderPitch->setValue(pFileObj->loopsAudioList->at(currSelected).semiTones);
    //    pFileObj->seek((double)40);
    pFileObj->seek(pFileObj->loopsAudioList->at(currSelected).beginLoop);
    ui->frameMark->addMark("Begin",pFileObj->loopsAudioList->at(currSelected).beginLoop,Qt::green);
    ui->frameMark->addMark("End",pFileObj->loopsAudioList->at(currSelected).endLoop,Qt::red);
//    qDebug() <<  pFileObj->loopsAudioList->at(currSelected).beginLoop;
}
