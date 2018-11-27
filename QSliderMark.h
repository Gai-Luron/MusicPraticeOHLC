#ifndef QSLIDERMARK_H
#define QSLIDERMARK_H

#include <QtGui>
#include <QSlider>
#include <QStyle>
#include<QList>




class QSliderMark : public QSlider
{
    Q_OBJECT
public:
    explicit QSliderMark(QWidget *parent = nullptr);
    void addMark(QString markName, int pos, Qt::GlobalColor color   );
    void removeMark(QString markName );
    void clearAllMark( );

private:
    struct markStruct {
        QString markName;
        int pos;
        Qt::GlobalColor color;
    };

    QList<markStruct>markList;
    int findMark( QString markName );


protected:
 void paintEvent(QPaintEvent *ev);
 void drawMark(int pos, Qt::GlobalColor color   );

signals:

public slots:
};

#endif // QSLIDERMARK_H
