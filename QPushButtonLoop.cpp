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
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(ShowContextMenu(const QPoint &)));
}
void QPushButtonLoop::ShowContextMenu(const QPoint &pos)
{
   QMenu contextMenu(tr("Context menu"), this);

   QAction action1(tr("Ajouter Boucle"), this);
   contextMenu.addAction(&action1);
   connect(&action1, SIGNAL(triggered()), this, SLOT(addShowContextMenu()));
   QAction action2(tr("Insérer Boucle"), this);
   connect(&action2, SIGNAL(triggered()), this, SLOT(insertShowContextMenu()));
   contextMenu.addAction(&action2);
   QAction action3(tr("Supprimer Boucle"), this);
   connect(&action3, SIGNAL(triggered()), this, SLOT(deleteShowContextMenu()));
   contextMenu.addAction(&action3);

   contextMenu.exec(mapToGlobal(pos));
}
void QPushButtonLoop::addShowContextMenu()
{
    emit contextMenuAction( loopContextMenu::ajouter,this->property("myId").toInt() );
}
void QPushButtonLoop::insertShowContextMenu()
{
    emit contextMenuAction( loopContextMenu::inserer,this->property("myId").toInt() );
}
void QPushButtonLoop::deleteShowContextMenu()
{
    emit contextMenuAction( loopContextMenu::supprimer,this->property("myId").toInt() );
}

void QPushButtonLoop::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::RightButton ){
        ShowContextMenu(event->pos());
        return;
    }
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
    emit clicked();
//    QPushButton::mousePressEvent(event);

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
    if (event->button() == Qt::LeftButton ){


        qle = new QLineEdit(this->text(), this);
        qle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        qle->setAlignment(Qt::AlignHCenter);

        hbl = new QVBoxLayout();
        hbl->setContentsMargins(8,4,8,4);
        this->setLayout(hbl);
        hbl->addWidget(qle);
        qle->setFocus();
        qle->connect(qle,SIGNAL(editingFinished()),this, SLOT(editedNameLoop()));

    }

}

void QPushButtonLoop::editedNameLoop( )
{
    this->setText(qle->text());
    emit editedNameLoop(this->property(("myId")).toInt(),this->text());

    qle->deleteLater();
    hbl->deleteLater();
}
