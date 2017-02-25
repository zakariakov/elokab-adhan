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

#include "adhkarform.h"
#include "ui_adhkarform.h"
//#include "dialogaddathkar.h"

#include <QSettings>

AdhkarForm::AdhkarForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdhkarForm)
{
ui->setupUi(this);
 loadSettings();
}

AdhkarForm::~AdhkarForm()
{
    delete ui;

}


void AdhkarForm::loadSettings()
{
    QSettings cg("elokab","adhan");
    cg.beginGroup("Adhan");

     ui->adhkarNotifyCheckBox->setChecked( cg.value("notifyAdhkar", true).toBool());
     ui->adhkarNotifySpinBox->setValue(cg.value("notifyAdhkarVal", 30).toInt());
     ui->adhkarOrderCheckBox->setChecked( cg.value("orderAdhkar", false).toBool());

     cg.endGroup();
}
void AdhkarForm::saveSettings()
{
    QSettings cg("elokab","adhan");
    cg.beginGroup("Adhan");


    cg.setValue("notifyAdhkar",  ui->adhkarNotifyCheckBox->isChecked());
    cg.setValue("notifyAdhkarVal", ui->adhkarNotifySpinBox->value());
    cg.setValue("orderAdhkar",ui->adhkarOrderCheckBox->isChecked());

  cg.endGroup();
}
