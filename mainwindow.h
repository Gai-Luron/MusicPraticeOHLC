#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include <QLayout>
#include <QPushButton>
#include <QTimer>
#include "QPushButtonLoop.h"
#include "processfile.h"
#include "recentfiles.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void triggerloopContextMenu(loopContextMenu,int);

    void editedNameLoop( int i, QString txt );

    void filesDropped(QStringList* fileList);

    void droppedLoopButOnOtherLoopBut( int orig, int dest);

    void doDelayedSetButtonLoop();

    void on_checkByPass_stateChanged(int arg1);

    void setCurrentTimePlayed(double i);

    void onButtonLoopClicked();

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

    void onContextMenuNewLoopClicked();

    void onContextMenuDeleteLoopClicked();

    void onContextMenuInsertLoopClicked();

    void setPlayConfigFromCurrentLoop();

    void slotPercChanged(double newPerc, QString markName);

    void requestPause();

    void loopRemaining(int nbLoop);


protected:
     void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    void deleteLayout(QLayout *item);
    void setButtonLoops();
    void displayLoopsInfo(int nb, int tot );
    QTimer *delayedSetButtonLoop;
    QTimer *endEditButtonLoop;
    int origButton = -1;
    int destButton = -1;
    processFile *pFileObj;
    bool flagUpdateSliderTimePlayed = true;
    recentFiles *rFiles = nullptr;

//    QLineEdit *qle;
//    QVBoxLayout *hbl;

};

#endif // MAINWINDOW_H
