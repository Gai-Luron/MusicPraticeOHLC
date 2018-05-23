#include "QPushButtonLoop.h"
#include <QDebug>
#include <QMimeData>
#include <QStringList>
#include <QFileInfo>
#include <QDragEnterEvent>
#include <QtWidgets>
#include <QDrag>



QPushButtonLoop::QPushButtonLoop(QWidget *parent) : QPushButton(parent)
{
    setAcceptDrops(true);
}

void QPushButtonLoop::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton )
       // && iconLabel->geometry().contains(event->pos()))
    {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setData("application/QPushbutton","");

        drag->setMimeData(mimeData);
        QPixmap image = QPixmap(":/icones/Loop-icon");
        drag->setPixmap(image.scaled(this->width(),this->height(),Qt::KeepAspectRatio));

        drag->exec();
    }
    this->setChecked(true);

}
void QPushButtonLoop::dragEnterEvent(QDragEnterEvent *event){

    if (event->mimeData()->hasFormat("application/QPushbutton")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }


}
void QPushButtonLoop::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/QPushbutton")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}
void QPushButtonLoop::dropEvent(QDropEvent *event){
    QVariant origMyId = event->source()->property("myId");
    QVariant destMyId = this->property("myId");

    if (event->mimeData()->hasFormat("application/QPushbutton")) {

        if (origMyId.isValid()) {
            if( origMyId.toInt() != destMyId.toInt() && destMyId.toInt() != 0){
                emit changePosButtonLoop(origMyId.toInt(), destMyId.toInt() ) ;
            }
            if (event->source() == this) {
                event->setDropAction(Qt::MoveAction);
                event->accept();
            }
            else {
                event->acceptProposedAction();
            }
        }
        else {
            event->ignore();
        }
    }
}
void QPushButtonLoop::mouseDoubleClickEvent(QMouseEvent* event){
    emit doubleClick(this->property(("myId")).toInt());
}
