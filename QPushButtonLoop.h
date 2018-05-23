#ifndef QPUSHBUTTONLOOP_H
#define QPUSHBUTTONLOOP_H

#include <QObject>
#include <QWidget>
#include<QTreeWidget>
#include <QPushButton>


class QPushButtonLoop : public QPushButton
{
    Q_OBJECT
public:
    QPushButtonLoop(QWidget *parent = 0);
    int myId;


protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event) override;
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent* event);


signals:
    void changePosButtonLoop(int origMyId, int destMyId ) ;
    void doubleClick(int myId);

};

#endif // QPUSHBUTTONLOOP_H
