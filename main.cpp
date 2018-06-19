#include "mainwindow.h"
#include <QApplication>
#include "frontSoundTouch.h"
#include <QDebug>
#include "SoundTouch.h"


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
