#include "buttoncolor.h"
#include <QColorDialog>
#include <QPainter>
#include <QDebug>
ButtonColor::ButtonColor(bool showAlphaChannel, QColor color, QWidget *parent) :
    QPushButton(parent),dlgc(0)
{
    isDlgRunning=false;
m_showAlphaChannel=showAlphaChannel;
        setIconSize(QSize(32,16));
         setMaximumSize(QSize(48,26));
         setMinimumSize(QSize(40,26));
        connect(this,SIGNAL(clicked()),this,SLOT(getColor()));
        setColor( color);
}



void ButtonColor::setColor(QColor color)
{

     m_color=color;
     QPixmap pix(32,16);
     pix.fill(Qt::transparent);
QPainter p;

p.begin(&pix);

p.fillRect(pix.rect(),m_color);
p.setPen(this->palette().shadow().color());
p.drawRect(pix.rect());
p.setPen(this->palette().light().color());
p.drawRect(-1,-1,32,16);
    // pix.fill(m_color);
     this->setIcon(pix);
}

void ButtonColor::getColor()
{
 /*
     QColor color;
   if(m_showAlphaChannel==true)
             color=QColorDialog::getColor(m_color,this,tr("Get Color"),QColorDialog::ShowAlphaChannel|Qt::NonModal);
else
          color=QColorDialog::getColor(m_color,this,tr("Get Color"));

        if(color.isValid()){
            setColor(color);
          emit colorChanged() ;
        }

   */
    if(dlgc){
        dlgc->activateWindow();
        return;
    }
    dlgc=new QColorDialog(m_color);


    if(m_showAlphaChannel==true)
        dlgc->setOption(QColorDialog::ShowAlphaChannel);

    connect(dlgc,SIGNAL(colorSelected(QColor)),this,SLOT(changeColor(QColor)));
    connect(dlgc,SIGNAL(rejected()),this,SLOT(dlgRejected()));

dlgc->setModal(true);
    dlgc->show();

}
void ButtonColor::changeColor(QColor color)
{
    if(dlgc){
      dlgc=0;
        delete dlgc;

    }
    isDlgRunning=false;
    setColor(color);
  emit colorChanged() ;
}
void ButtonColor::dlgRejected()
{
    if(dlgc){
       dlgc=0;
        delete dlgc;

    }

}
