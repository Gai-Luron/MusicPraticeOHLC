#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processfile.h"
#include "configaudiofile.h"
#include "recentfiles.h"
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QPushButton>



processFile *pFileObj;
configAudioFile *cfAF;
recentFiles *rFiles;

bool flagUpdateSliderTimePlayed = true;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->currentFileRead->setText(tr("Pas de fichier"));
    ui->currentPathRead->setText("");

    cfAF = new configAudioFile();

    pFileObj =new processFile();
    pFileObj->bypassStrech = false;

    connect(pFileObj, SIGNAL(processed(float)),this, SLOT(setCurrentTimePlayed(float)));

    ui->valueTempo->setText(QString::number(ui->sliderTempo->value()));
    ui->valueSemiTone->setText(QString::number(ui->sliderPitch->value()));

    updateRecentFilesWidget();


}
void MainWindow::setButtonLoops(){
    static QPushButton *pButton[100];

    deleteLayout(ui->layoutLoopAll);
    deleteLayout( ui->gridLayoutLoops);

    listLoopsButtons.clear();
    for( int i = 0; i < cfAF->currConfigAudioFile.nbLoops;i++){
        pButton[i] = new QPushButton(this);
        pButton[i]->setProperty("myId",i);
        pButton[i]->setText(cfAF->currConfigAudioFile.loopsAudio[i].loopName);
        if( i == 0){
            ui->layoutLoopAll->addWidget(pButton[i]);
        }
        else{
            ui->gridLayoutLoops->addWidget(pButton[i],(i-1)/4,(i-1)%4);
        }
        connect(pButton[i],SIGNAL(clicked()), this,SLOT(on_push_loop()));
    }
}
void MainWindow::deleteLayout(QLayout *item){
    while (item->count() > 0) {
        QLayoutItem *child = item->takeAt(0);
        if (child == 00) continue;
        if (child->layout() != 0) deleteLayout(child->layout());
        if (child->widget()) delete child->widget();
        delete child;
    }
}
void MainWindow::on_push_loop(){
    QVariant myId = sender()->property("myId");
     if (myId.isValid()) {
       int idx = myId.toInt();
       qDebug() << "Index" << idx << " nom du Boutton Pressé = " << cfAF->currConfigAudioFile.loopsAudio[idx].loopName;
     }

}
void MainWindow::updateRecentFilesWidget(){
    QStringList headers;
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
void MainWindow::setCurrentTimePlayed(float nb)
{
    if( flagUpdateSliderTimePlayed ){
        ui->currentTimePlayed->setValue(nb*100);
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_checkByPass_stateChanged(int arg1)
{
    if( arg1 == Qt::Unchecked)
        pFileObj->bypassStrech = true;
    else
        pFileObj->bypassStrech = false;

}



void MainWindow::on_currentTimePlayed_sliderPressed()
{
    flagUpdateSliderTimePlayed = false;
}

void MainWindow::on_currentTimePlayed_sliderReleased()
{
    flagUpdateSliderTimePlayed = true;
    pFileObj->seek((float)((float)ui->currentTimePlayed->value()/(float)100));

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
    ui->valueSemiTone->setText(QString::number(ui->sliderPitch->value()));
    pFileObj->setPitchSemiTones(ui->sliderPitch->value());
}

void MainWindow::on_sliderTempo_valueChanged(int value)
{
    ui->valueTempo->setText(QString::number(ui->sliderTempo->value()));
    pFileObj->setTempo(ui->sliderTempo->value());

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
    pFileObj->openSoundFile(fileName);
    cfAF->setFilename(fileName);
    setButtonLoops();
    if( launchPlay ){
        pFileObj->play();
        ui->actionPlay->setChecked(true);
    }

}
void MainWindow::on_recentFilesWidget_itemActivated(QTreeWidgetItem *item, int column)
{
    QString fileName;
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

    if( flagFirst ){
        flagFirst = false;
        ui->currentFileRead->setText(current->text(0));
        ui->currentPathRead->setText(current->text(2));
        fileName = current->text(3);
        pFileObj->openSoundFile(fileName);
        cfAF->setFilename(fileName);
        setButtonLoops();
    }

}
