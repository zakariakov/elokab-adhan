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


#ifndef ADHKARFORM_H
#define ADHKARFORM_H

#include <QWidget>

namespace Ui {
    class AdhkarForm;
}

class AdhkarForm : public QWidget
{
    Q_OBJECT

public:

    explicit AdhkarForm(QWidget *parent = 0);
    ~AdhkarForm();
void saveSettings();
private slots:
 void loadSettings();

private:
    Ui::AdhkarForm *ui;

   void    creatdataBook(const QString &fileName);
};

#endif // ADHKARFORM_H
