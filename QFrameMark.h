#ifndef QFRAMEMARK_H
#define QFRAMEMARK_H

#include <QtGui>
#include <QLabel>
#include <QStyle>
#include<QList>
#include <QFrame>

#ifndef QMARK_H
#define QMARK_H

#include <QtGui>
#include <QStyle>
#include<QList>
#include <QFrame>

class QMark : public QFrame
{
    Q_OBJECT
public:
    explicit QMark(QWidget *parent = nullptr);
    Qt::GlobalColor color;
    QString markName;

private:
    enum class typOrientation
      {
         vertical,
         horizontal
      };
    typOrientation orientation;
    QPoint offset;

protected:
 void paintEvent(QPaintEvent *ev);
 void mousePressEvent(QMouseEvent *event);
 void mouseMoveEvent(QMouseEvent *event);

signals:
    void posChanged(int, QString);


public slots:
};

#endif // QMARK_H


class QFrameMark : public QFrame
{
    Q_OBJECT
public:
    explicit QFrameMark(QWidget *parent = nullptr);
    void addMark(QString markName, double perc, Qt::GlobalColor color   );
    void updatePosMark(QString markName, double perc );
    void removeMark(QString markName );
    void clearAllMark( );

private:
    struct markStruct {
        QString markName;
        double perc;
        Qt::GlobalColor color;
        QMark *Qm;
    };

    QList<markStruct>markList;
    int findMark( QString markName );
    enum class typOrientation
      {
         vertical,
         horizontal
      };
    typOrientation orientation;

protected:
 void paintEvent(QPaintEvent *ev);
 void resizeEvent( QResizeEvent * event);
 void drawMark(int pos, Qt::GlobalColor color   );
 int percToPos( double perc );
 double posToPerc( int pos );
 void moveMark( int idx );



signals:
    void percChanged(double, QString);

public slots:
    void slotPosChanged(int, QString);
};

#endif // QFRAMEMARK_H


