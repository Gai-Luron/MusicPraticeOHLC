#-------------------------------------------------
#
# Project created by QtCreator 2018-04-14T18:29:22
#
#-------------------------------------------------

QT      += core gui
QT      += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPraticeOHLC
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/include

LIBS += $$_PRO_FILE_PWD_/lib/libsndfile-1.dll
LIBS += $$_PRO_FILE_PWD_/lib/portaudio.dll
LIBS += $$_PRO_FILE_PWD_/lib/libSoundTouch.a

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        recentfiles.cpp \
    configaudiofile.cpp \
    QRecentsFilesTree.cpp \
    QPushButtonLoop.cpp \
    frontSoundTouch.cpp \
    processfile.cpp \
    QFrameMark.cpp

HEADERS += \
        mainwindow.h \
        recentfiles.h \
    configaudiofile.h \
    QRecentsFilesTree.h \
    QPushButtonLoop.h \
    frontSoundTouch.h \
    processfile.h \
    QFrameMark.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    README.md

RESOURCES += \
    res.qrc
