#ifndef QPUSHBUTTONLOOP_H
#define QPUSHBUTTONLOOP_H

#include <QObject>
#include <QWidget>
#include<QTreeWidget>
#include <QPushButton>
#include <QVBoxLayout>


enum class loopContextMenu{
    ajouter,
    inserer,
    supprimer
};
class QPushButtonLoop : public QPushButton
{
    Q_OBJECT
public:
    QPushButtonLoop(QWidget *parent = 0);
    int myId;
private:
    QLineEdit *qle;
    QVBoxLayout *hbl;


protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event) override;
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent* event);



signals:
    void changePosButtonLoop(int origMyId, int destMyId ) ;
//    void doubleClick(int myId);
    void editedNameLoop(int,QString);
    void contextMenuAction(loopContextMenu val, int myId);

private slots:
    void ShowContextMenu(const QPoint &pos);
    void addShowContextMenu();
    void insertShowContextMenu();
    void deleteShowContextMenu();
    void editedNameLoop( );

};

#endif // QPUSHBUTTONLOOP_H
