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

    connect(pFileObj, SIGNAL(processed(float)),this, SLOT(setCurrentTimePlayed(float)));
    ui->valueTempo->setText(QString::number(ui->sliderTempo->value()));
    ui->valueSemiTone->setText(QString::number(ui->sliderPitch->value()));

}
void MainWindow::setCurrentTimePlayed(float nb)
{
    if( flagUpdateSliderTimePlayed )
        ui->currentTimePlayed->setValue(nb*100);
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
