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

#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H
#include "location.h"
#include "display.h"
#include "notification.h"
#include "adhkarform.h"
#include <QDialog>

namespace Ui {
class DialogConfig;
}

class DialogConfig : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogConfig(QWidget *parent = 0);
    ~DialogConfig();
signals:
  void  accepteChange();
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::DialogConfig *ui;
    LocationForm *m_location;
    NotificationForm *m_notification;
    DisplayForm *m_display;
   AdhkarForm *m_adhkar;
};

#endif // DIALOGCONFIG_H
