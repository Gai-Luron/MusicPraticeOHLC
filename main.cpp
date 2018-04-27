#include "mainwindow.h"
#include <QApplication>
#include "processfile.h"
#include <QDebug>
#include "soundtouch_config.h"
#include "SoundTouch.h"
#include "SoundTouchDLL.h"


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
