#include "QRecentsFilesTree.h"
#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QStringList>
#include <QFileInfo>

QStringList allowedExtension;
QStringList listOfFilesDropped;


QRecentsFilesTree::QRecentsFilesTree(QWidget *parent) : QTreeWidget(parent)
{
    setAcceptDrops(true);
}
void QRecentsFilesTree::dragEnterEvent(QDragEnterEvent *event){

    if (event->mimeData()->hasFormat("text/uri-list") && isAllowedExtension(event->mimeData()->urls()) ){
        event->acceptProposedAction();
    }
}
void QRecentsFilesTree::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list") && isAllowedExtension(event->mimeData()->urls()) ){
        event->acceptProposedAction();
    }
}
void QRecentsFilesTree::dropEvent(QDropEvent *event){
    listOfFilesDropped.clear();

    QFileInfo QF;
    QList<QUrl> urlList = event->mimeData()->urls();
    for (int i = 0; i < urlList.size() ; ++i){
        QString currFile = urlList.at(i).toString();
        QF.setFile(currFile);
        if( allowedExtension.contains(QF.suffix())){
            listOfFilesDropped << currFile.mid(8);
        }
    }
    if( listOfFilesDropped.count() != 0)
        emit filesListDropped(&listOfFilesDropped);
}
bool QRecentsFilesTree::isAllowedExtension(QList<QUrl> urlList){
    QFileInfo QF;
    for (int i = 0; i < urlList.size() ; ++i){
        QString currFile = urlList.at(i).toString();
        QF.setFile(currFile);
        if( allowedExtension.contains(QF.suffix())){
            return true;
        }
    }
    return false;
}
 void QRecentsFilesTree::addAllowedExtension( QString ext ){
     allowedExtension << ext;
}
