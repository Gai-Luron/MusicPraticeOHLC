#include <QtGui>
#include<QDebug>
#include <QFrame>
#include <QSlider>
#include <QColor>
#include "QFrameMark.h"



QFrameMark::QFrameMark(QWidget *parent) : QFrame(parent)
{
    if ( this->height() > this->width())
        this->orientation = typOrientation::vertical;
    else
        this->orientation = typOrientation::horizontal;
}
void QFrameMark::resizeEvent( QResizeEvent * event) {
    for( int i = 0; i < markList.count(); i++ )
        moveMark(i);
    repaint();


}
void QFrameMark::moveMark( int idx ){

    if ( this->orientation == typOrientation::vertical ){
        markList[idx].Qm->move(width()-10,percToPos(markList[idx].perc));
    }
    else{
        markList[idx].Qm->move(percToPos(markList[idx].perc),height()-10);
    }

}
int QFrameMark::percToPos( double perc ) {

    if ( this->orientation == typOrientation::vertical )
        return int(((height() - 12) * perc/100)+6-5);
    else
        return int(((width() - 12) * perc/100)+6-5);
}
double QFrameMark::posToPerc( int pos ) {

    if ( this->orientation == typOrientation::vertical )
        return (pos-6+5)/(height() - 12) * 100;
    else
        return ((double(pos)-6.0+5.0)*100.0)/(double(width()) - 12.0);
}

void QFrameMark::paintEvent(QPaintEvent *ev) {

    QFrame::paintEvent(ev);

 }
int QFrameMark::findMark( QString markName ){

    for( int i = 0; i < markList.count(); i++ ){
        if ( markList.at(i).markName == markName ){
            return i;
        }
    }
    return -1;
}
void QFrameMark::clearAllMark(){
    markList.clear();
    this->update();
}

void QFrameMark::removeMark(QString markName ){
    int posMark = findMark( markName );
    if( posMark != -1 ){
        markList.removeAt(posMark);
        this->update();
    }
}
void QFrameMark::updatePosMark(QString markName, double perc ){
    int posMark = findMark( markName );
    if(  posMark != -1 ){
        addMark(markList[posMark].markName, perc, markList[posMark].color);
    }
}

void QFrameMark::addMark(QString markName, double perc, Qt::GlobalColor color   ){

    int posMark = findMark( markName );

    if(  posMark != -1 ){
        markList[posMark].markName = markName;
        markList[posMark].perc = perc;
        markList[posMark].color = color;
        delete markList[posMark].Qm;
    }
    else{
        markStruct ma;
        ma.markName = markName;
        ma.perc = perc;
        ma.color= color ;
        markList.append( ma );
        posMark = findMark(markName);
    }
    markList[posMark].Qm = new QMark(this);
    //    Ql->setStyleSheet("border: 1px solid black;color: blue; background-color: red");
//    markList[posMark].Ql->setStyleSheet("border: 1px solid black");
    markList[posMark].Qm->markName = markName;
    markList[posMark].Qm->setFixedWidth(10);
    markList[posMark].Qm->setFixedHeight(10);
    markList[posMark].Qm->color = color;
    markList[posMark].Qm->show();
    moveMark( posMark );
    connect( markList[posMark].Qm, SIGNAL(posChanged(int,QString)), this, SLOT(slotPosChanged(int,QString)) );

}
void QFrameMark::slotPosChanged(int newPos, QString markName){
    int idx = findMark(markName);
    double newPerc = posToPerc(newPos);
    if( newPerc < 0 )
        newPerc = 0;
    if( newPerc > 100 )
        newPerc = 100;
    markList[idx].perc = newPerc;
    markList[idx].Qm->move(percToPos(markList[idx].perc),height()-10);
    emit percChanged(newPerc, markName );



}


//
//
// QMark derivated from QFrame, Draw QFrame with color triangle
//
//
QMark::QMark(QWidget *parent) : QFrame(parent){
}
void QMark::paintEvent(QPaintEvent *ev) {
    QPainter painter(this);
    QPen pen;
    QBrush qb;



    qb.setStyle(Qt::SolidPattern);
    qb.setColor(color);
    pen.setBrush(Qt::black);

    painter.setPen(pen);
    painter.setBrush(qb);

    QPointF points[3] = {
        QPointF( 0, 0),
        QPointF( width(), 0),
        QPointF(5, height())
    };
    painter.drawPolygon(points, 3);

    QFrame::paintEvent(ev);
}
void QMark::mousePressEvent(QMouseEvent *event)
{
    offset = event->pos();
}
void QMark::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QPoint newOffset;
        newOffset = mapToParent(event->pos() - offset);
        newOffset.setY(0);

        this->move(newOffset);
        emit posChanged(newOffset.x(), markName );

    }
}
