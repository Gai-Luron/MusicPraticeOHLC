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
    connect(pFileObj, SIGNAL(requestPause()),this, SLOT(requestPause()));
    connect(pFileObj, SIGNAL(loopRemaining(int)),this, SLOT(loopRemaining(int)));

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

    if( pFileObj->currentLoop() != 0 ){ // Not The All Loop
        if( markName == "Begin"){
            if( newPerc < pFileObj->endLoop() )
                 pFileObj->setBeginLoop(newPerc);
            else
                ui->frameMark->updatePosMark("Begin",pFileObj->beginLoop());
        }
        if( markName == "End"){
            if( newPerc > pFileObj->beginLoop() )
                pFileObj->setEndLoop( newPerc );
            else
                ui->frameMark->updatePosMark("End",pFileObj->endLoop());

        }
        pFileObj->saveConfig();
    }
    else{
        ui->frameMark->updatePosMark("Begin",pFileObj->beginLoop());
        ui->frameMark->updatePosMark("End",pFileObj->endLoop());
    }

}
void MainWindow::triggerloopContextMenu(loopContextMenu lc,int idx)
{
    QString action;
    loopAudio tmpLoopAudio;
    switch( lc )
    {
        case loopContextMenu::ajouter:
            action = "Ajouter";
            tmpLoopAudio.loopName = tr("Nouveau");
            tmpLoopAudio.beginLoop = 0;
            tmpLoopAudio.endLoop = 100;
            tmpLoopAudio.tempo = 100;
            tmpLoopAudio.semiTones = 0;
            pFileObj->appendLoop(tmpLoopAudio);
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
                pFileObj->insertLoop(tmpLoopAudio,idx);
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
                pFileObj->removeLoop(idx);
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
    pFileObj->setLoopName(txt,i);
    pFileObj->saveConfig();
    pFileObj->setCurrentLoop( i );
}

void MainWindow::droppedLoopButOnOtherLoopBut( int orig, int dest)
    {

    pFileObj->moveLoop(orig,dest);
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


    for( int i = 0; i < pFileObj->countLoop();i++){
        but = new QPushButtonLoop(this);
        but->setObjectName("Bloop" + QString::number(i));
        but->setProperty("myId",i);
        but->setText(pFileObj->loopName(i));
        but->setCheckable(true);
        but->setAutoExclusive(true);
        if( i == 0){
            ui->layoutLoopAll->addWidget(but);
        }
        else{
            ui->gridLayoutLoops->addWidget(but,(i-1)/4,(i-1)%4);
        }
        but->setChecked(i == pFileObj->currentLoop());
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
       pFileObj->setCurrentLoop(idx);
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

    if( flagUpdateSliderTimePlayed ){
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
    pFileObj->setTempoDirect(ui->sliderTempo->value());

}

void MainWindow::on_sliderPitch_valueChanged(int value)
{

    pFileObj->setSemiTones(value);
    pFileObj->saveConfig();

    ui->valueSemiTone->setText(QString::number(value));
}

void MainWindow::on_sliderTempo_valueChanged(int value)
{

    pFileObj->setTempo(value);
    pFileObj->saveConfig();

    ui->valueTempo->setText(QString::number(value));
}


void MainWindow::on_valueSemiTone_editingFinished()
{
    if( ui->valueSemiTone->text().toInt() < ui->sliderPitch->minimum() )
        ui->valueSemiTone->setText(QString::number(ui->sliderPitch->minimum()));
    if( ui->valueSemiTone->text().toInt() > ui->sliderPitch->maximum() )
        ui->valueSemiTone->setText(QString::number(ui->sliderPitch->maximum()));
    ui->sliderPitch->setValue(ui->valueSemiTone->text().toInt());
    pFileObj->setPitchSemiTonesDirect(ui->sliderPitch->value());


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
void MainWindow::requestPause(){
    on_actionPause_triggered();
    qDebug("Request Pause");
}
void MainWindow::on_actionPlay_triggered()
{
    displayLoopsInfo(1, pFileObj->nbLoop());
    if ( ui->actionPlay->isChecked()){
        if ( !pFileObj->play() ){
            ui->actionPlay->setChecked(false);
        }

    }
    else{
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
    triggerloopContextMenu(loopContextMenu::supprimer,pFileObj->currentLoop());
}

void MainWindow::onContextMenuInsertLoopClicked()
{
    triggerloopContextMenu(loopContextMenu::inserer,pFileObj->currentLoop());
}
void MainWindow::setPlayConfigFromCurrentLoop(){
    ui->sliderTempo->setValue(pFileObj->tempo());
    ui->sliderPitch->setValue(pFileObj->semiTones());
    //    pFileObj->seek((double)40);
    pFileObj->seek(pFileObj->beginLoop());
    ui->frameMark->addMark("Begin",pFileObj->beginLoop(),Qt::green);
    ui->frameMark->addMark("End",pFileObj->endLoop(),Qt::red);
    displayLoopsInfo( 1,pFileObj->nbLoop());
//    qDebug() <<  pFileObj->beginLoop();
}
void MainWindow::loopRemaining(int nbLoop ){
    displayLoopsInfo(pFileObj->nbLoop()-nbLoop+1, pFileObj->nbLoop());
}
void MainWindow::displayLoopsInfo(int nb, int tot ){
    if( tot == -1)
        ui->infoLoop->setText( "Loop(s): infinite" );
    else
        ui->infoLoop->setText( "Loop(s) " + QString::number( nb ) + "/" + QString::number(tot));

}
