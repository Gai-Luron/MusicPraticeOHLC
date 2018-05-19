#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include <QLayout>
#include <QPushButton>
#include <QTimer>
#include "QPusButtonLoop.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void filesDropped(QStringList* fileList);

    void droppedLoopButOnOtherLoopBut( int orig, int dest);

    void doSwapButtonLoop();

    void on_checkByPass_stateChanged(int arg1);

    void setCurrentTimePlayed(float i);

    void pushButtonLoop();

    void on_currentTimePlayed_sliderPressed();

    void on_currentTimePlayed_sliderReleased();

    void on_valueTempo_editingFinished();

    void on_sliderPitch_valueChanged(int value);

    void on_sliderTempo_valueChanged(int value);

    void on_valueSemiTone_editingFinished();

    void updateRecentFilesWidget();

    void on_recentFilesWidget_itemActivated(QTreeWidgetItem *item, int column);

    void on_openFile_triggered();

    void startNewaudioFile( QString fileName, bool launchPlay );

    void on_actionPlay_triggered();

    void on_actionPause_triggered();

    void on_recentFilesWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);


    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::MainWindow *ui;
    QList<QPushButton> listLoopsButtons;
    void deleteLayout(QLayout *item);
    void setButtonLoops();
    QTimer *swapButtonLoop;
    int origButton = -1;
    int destButton = -1;

};

#endif // MAINWINDOW_H
