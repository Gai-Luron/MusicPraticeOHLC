#ifndef QRECENTSFILESTREE_H
#define QRECENTSFILESTREE_H

#include <QObject>
#include <QWidget>
#include<QTreeWidget>

class QRecentsFilesTree : public QTreeWidget
{
    Q_OBJECT
public:
    QRecentsFilesTree(QWidget *parent = 0);
    void addAllowedExtension( QString ext );


private:
    bool isAllowedExtension(QList<QUrl> urlList);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event) override;

signals:
    void filesListDropped(QStringList *QSL);

};

#endif // QRECENTSFILESTREE_H
