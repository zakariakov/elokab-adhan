/***************************************************************************
 *   elokab Copyright (C) 2014 AbouZakaria <yahiaui@gmail.com>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef BUTTONCOLOR_H
#define BUTTONCOLOR_H

#include <QPushButton>
#include <QColorDialog>
class ButtonColor : public QPushButton
{
          Q_OBJECT
public:

          explicit ButtonColor(bool showAlphaChannel= false,QColor color=QColor("black"),  QWidget *parent = 0);

signals:
          void colorChanged() ;
public slots:

          void setColor(QColor color);
          QColor color(){
                    return m_color;
          }

private:
          bool isDlgRunning;
          QColorDialog *dlgc;
          QColor m_color;
          bool m_showAlphaChannel;
private slots:
          void getColor();

          void changeColor(QColor color);
          void dlgRejected();
};

#endif // BUTTONCOLOR_H
