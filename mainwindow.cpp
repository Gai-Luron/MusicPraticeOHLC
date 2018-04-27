#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processfile.h"
#include "recentfiles.h"
#include <QDebug>



processFile *pFileObj;
recentFiles *rFiles;
bool flagUpdateSliderTimePlayed = true;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    pFileObj =new processFile();
    pFileObj->bypassStrech = false;
    rFiles = new recentFiles();

    connect(pFileObj, SIGNAL(processed(int)),this, SLOT(setCurrentTimePlayed(int)));
    ui->valueTempo->setText(QString::number(ui->sliderTempo->value()));

}
void MainWindow::setCurrentTimePlayed(int nb)
{
    if( flagUpdateSliderTimePlayed )
        ui->currentTimePlayed->setValue(nb);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_goSound1_clicked()
{
    QString fileName = "D:/MesDocuments/Dev/MusicPraticeOHLC/Debug/debug/BYTShort.wav";
    rFiles->addFile(fileName);
    pFileObj->openSoundFile(fileName);
    pFileObj->play();
}

void MainWindow::on_goSound2_clicked()
{
    QString fileName = "D:/MesDocuments/Dev/MusicPraticeOHLC/Debug/debug/BYTDessay.wav";
    rFiles->addFile(fileName);
    pFileObj->openSoundFile(fileName);
    pFileObj->play();
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = "D:/MesDocuments/Dev/MusicPraticeOHLC/Debug/debug/BYT883.ogg";
    rFiles->addFile(fileName);
    pFileObj->openSoundFile(fileName);
    pFileObj->play();

}

void MainWindow::on_sliderTempo_actionTriggered(int action)
{
    ui->valueTempo->setText(QString::number(ui->sliderTempo->value()));
    pFileObj->setTempo(ui->sliderTempo->value());
}


void MainWindow::on_checkByPass_stateChanged(int arg1)
{
    if( arg1 == Qt::Unchecked)
        pFileObj->bypassStrech = true;
    else
        pFileObj->bypassStrech = false;

}

void MainWindow::on_butPlay_clicked()
{
    pFileObj->play();
}

void MainWindow::on_butPause_clicked()
{
    pFileObj->pause();
}


void MainWindow::on_currentTimePlayed_sliderPressed()
{
    flagUpdateSliderTimePlayed = false;
}

void MainWindow::on_currentTimePlayed_sliderReleased()
{
    flagUpdateSliderTimePlayed = true;
    qDebug() << ui->currentTimePlayed->value();

}

void MainWindow::on_valueTempo_editingFinished()
{
    if( ui->valueTempo->text().toInt() < 40 )
        ui->valueTempo->setText("40");
    if( ui->valueTempo->text().toInt() > 200 )
        ui->valueTempo->setText("200");
    ui->sliderTempo->setValue(ui->valueTempo->text().toInt());
    pFileObj->setTempo(ui->sliderTempo->value());

}
