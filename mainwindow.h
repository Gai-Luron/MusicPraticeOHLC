#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_goSound1_clicked();

    void on_goSound2_clicked();

    void on_pushButton_clicked();

    void on_checkByPass_stateChanged(int arg1);

    void on_butPlay_clicked();

    void on_butPause_clicked();

    void setCurrentTimePlayed(float i);


    void on_currentTimePlayed_sliderPressed();

    void on_currentTimePlayed_sliderReleased();

    void on_valueTempo_editingFinished();

    void on_sliderPitch_valueChanged(int value);

    void on_sliderTempo_valueChanged(int value);

    void on_valueSemiTone_editingFinished();


private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
