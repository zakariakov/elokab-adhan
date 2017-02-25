/***************************************************************************
 *   Copyright (C) 2010-2011 by Amine Roukh     <amineroukh@gmail.com>     *
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


#ifndef DISPLAY_H
#define DISPLAY_H
#include "buttoncolor/buttoncolor.h"
#include <QWidget>

#include "ui_displayform.h"

class DisplayForm : public QWidget
{
    Q_OBJECT
public:
    DisplayForm(QWidget *parent = 0);
   ~DisplayForm();
void saveSettings();
    
 /*
    void setShowNextPrayer(bool enabled);
    void setShowCountdown(bool enabled);
    void setIgnoreHour(bool enabled);
    void setIgnoreSecs(bool enabled);
    void setShowElapsed(bool enabled);
    void setIconEffect(bool enabled);
    void setAmpmFormat(bool enabled);
    */
signals:
  void  accepteChange();
private slots:
 // void on_buttonBox_clicked(QAbstractButton *button);
  void loadSettings();


private:
    Ui::DisplayForm m_displayUi;
       void setPrayersToShow(QList<bool> prayersToShowList);
       QList<bool> prayersToShow();
       bool showNextPrayer();
       bool showCountdown();
       bool ignoreHour();
       bool ignoreSecs();
       bool showElapsed();
    //   bool iconEffect();
       bool ampmFormat();

       //----colors----------
       ButtonColor *btnColorBackground;
  ButtonColor *btnColorText;
    ButtonColor *btnColorNext;
      ButtonColor *btnColorPrev;
};

#endif
