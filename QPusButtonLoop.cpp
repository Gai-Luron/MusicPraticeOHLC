#include "QPusButtonLoop.h"
#include <QDebug>
#include <QMimeData>
#include <QStringList>
#include <QFileInfo>
#include <QDragEnterEvent>
#include <QtWidgets>


void QPushButtonLoop::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton )
       // && iconLabel->geometry().contains(event->pos()))
    {

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setData("application/QPushbutton","");
        drag->setMimeData(mimeData);
        //        QPixmap image = QPixmap(this->width(),this->height());
            QPixmap image = QPixmap(":/icones/Loop-icon");
        drag->setPixmap(image.scaled(this->width(),this->height(),Qt::KeepAspectRatio));

        Qt::DropAction dropAction = drag->exec();
    }
}
QPushButtonLoop::QPushButtonLoop(QWidget *parent) : QPushButton(parent)
{
    setAcceptDrops(true);
}
void QPushButtonLoop::dragEnterEvent(QDragEnterEvent *event){

    if (event->mimeData()->hasFormat("application/QPushbutton") ){
        event->acceptProposedAction();
    }
}
void QPushButtonLoop::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/QPushbutton")  ){
        event->acceptProposedAction();
    }
}
void QPushButtonLoop::dropEvent(QDropEvent *event){
    QVariant origMyId = event->source()->property("myId");
    QVariant destMyId = this->property("myId");

    if (origMyId.isValid()) {
        if( origMyId.toInt() != destMyId.toInt() && destMyId.toInt() != 0){
            emit changePosButtonLoop(origMyId.toInt(), destMyId.toInt() ) ;
            qDebug() << origMyId.toInt() << " Sur " << destMyId.toInt();
        }
    }
}
