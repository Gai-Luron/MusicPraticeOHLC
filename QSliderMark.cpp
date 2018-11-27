#include <QtGui>
#include <QSlider>
#include <QColor>
#include "QSliderMark.h"



QSliderMark::QSliderMark(QWidget *parent) : QSlider(parent)
{
}
void QSliderMark::paintEvent(QPaintEvent *ev) {

    for( int i = 0; i < markList.count(); i++ ){
        drawMark( markList.at(i).pos, markList.at(i).color);
    }
    QSlider::paintEvent(ev);


 }
void QSliderMark::drawMark( int pos, Qt::GlobalColor color   ){
    QPainter painter(this);
    QPen pen;
    QBrush qb;
    int taille;

    if ( this->orientation() == Qt::Vertical)
        taille = height();
    else
        taille = width();


    int position = QStyle::sliderPositionFromValue(minimum(),
                                                  maximum(),
                                                  pos,
                                                  taille-11)+5;
    qb.setStyle(Qt::SolidPattern);
    qb.setColor(color);
    pen.setBrush(Qt::black);

    painter.setPen(pen);
    painter.setBrush(qb);



    if ( this->orientation() == Qt::Vertical){

        QPointF points[3] = {
            QPointF( 0,position-5),
            QPointF(0, position+5),
            QPointF(5, position),
        };
        painter.drawPolygon(points, 3);
    }
    else{
        QPointF points[3] = {
            QPointF( position-5, 0.0),
            QPointF(position+5, 0),
            QPointF(position, 5),
        };
        painter.drawPolygon(points, 3);
    }
}
int QSliderMark::findMark( QString markName ){

    for( int i = 0; i < markList.count(); i++ ){
        if ( markList.at(i).markName == markName ){
            return i;
        }
    }
    return -1;
}
void QSliderMark::clearAllMark(){
    markList.clear();
    this->update();
}

void QSliderMark::removeMark(QString markName ){
    int posMark = findMark( markName );
    if( posMark != -1 ){
        markList.removeAt(posMark);
        this->update();
    }
}

void QSliderMark::addMark(QString markName, int pos, Qt::GlobalColor color   ){

    int posMark = findMark( markName );

    if(  posMark != -1 ){
        markList[posMark].markName = markName;
        markList[posMark].pos = pos;
        markList[posMark].color = color;
        return;
    }
    else{
        markStruct ma;
        ma.markName = markName;
        ma.pos = pos;
        ma.color= color ;
        markList.append( ma );
    }
}
