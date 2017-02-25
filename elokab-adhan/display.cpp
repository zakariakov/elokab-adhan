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


#include "display.h"

#include <QDebug>
#include <QSettings>
DisplayForm::DisplayForm(QWidget *parent) :
    QWidget(parent)
{
    m_displayUi.setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    btnColorBackground=new   ButtonColor(true,QColor(250, 255, 212, 255),this);
    btnColorText=new ButtonColor;
btnColorNext=new ButtonColor;
btnColorPrev=new ButtonColor;
   m_displayUi.hlLayoutBackground->addWidget(btnColorBackground);
   m_displayUi.hlLayoutText->addWidget(btnColorText);
   m_displayUi.hlLayoutNext->addWidget(btnColorNext);
   m_displayUi.hlLayoutPrev->addWidget(btnColorPrev);
    loadSettings();
}
DisplayForm::~DisplayForm()
{
    //delete m_displayUi;
}

void DisplayForm::loadSettings()
{
    QSettings cg("elokab","adhan");
    cg.beginGroup("Adhan");



    QList<QVariant> m_prayersToShowList= cg.value("prayersToShowList", QList<QVariant>() << true << false << true
                                                  << true << true << true).toList();
    QList<bool> prayersToShowList;
    foreach (QVariant r, m_prayersToShowList) {
        prayersToShowList.append(r.toBool());
    }
    this->setPrayersToShow(prayersToShowList);

    m_displayUi.showNextRadioButton->setChecked( cg.value("showNextPrayer", false).toBool());
    m_displayUi.countdownRadioButton->setChecked(cg.value("showCountdown", true).toBool());
    m_displayUi.showElapsedCheckBox->setChecked( cg.value("showElapsed", true).toBool());
    m_displayUi.ignoreHourCheckBox->setChecked(cg.value("ignoreHour", true).toBool());
    m_displayUi.ignoreSecsCheckBox->setChecked(cg.value("ignoreSecs", true).toBool());
  //  m_displayUi.iconEffectCheckBox->setChecked( cg.value("iconEffect", false).toBool());
    m_displayUi.ampmCheckBox->setChecked( cg.value("ampmFormat", false).toBool());
//----------colors-----------------
    btnColorBackground->setColor(cg.value("ColorBackground",QColor(250, 255, 212, 255)).value<QColor>());
    btnColorText->setColor(cg.value("ColorText",QColor(0, 0, 0, 255)).value<QColor>());
    btnColorNext->setColor(cg.value("ColorNext",QColor(0, 0, 255, 255)).value<QColor>());
    btnColorPrev->setColor(cg.value("ColorPrev",QColor(255, 0, 0, 255)).value<QColor>());
    cg.endGroup();
}
void DisplayForm::saveSettings()
{
    QSettings cg("elokab","adhan");
    cg.beginGroup("Adhan");

    QList<QVariant>  m_prayersToShowList;
    foreach (bool r, prayersToShow()) {
        m_prayersToShowList.append(r);
    }


    cg.setValue("prayersToShowList", m_prayersToShowList);
    cg.setValue("showNextPrayer", showNextPrayer());
    cg.setValue("showCountdown", showCountdown());
    cg.setValue("showElapsed", showElapsed());
    cg.setValue("ignoreHour", ignoreHour());
    cg.setValue("ignoreSecs", ignoreSecs());
  //  cg.setValue("iconEffect", iconEffect());
    cg.setValue("ampmFormat", ampmFormat());
//----------colors-----------------
    cg.setValue("ColorBackground", btnColorBackground->color());
    cg.setValue("ColorText", btnColorText->color());
    cg.setValue("ColorNext", btnColorNext->color());
    cg.setValue("ColorPrev", btnColorPrev->color());
    cg.endGroup();
}

QList<bool> DisplayForm::prayersToShow()
{
    QList<bool> prayersToShowList;
    
    prayersToShowList.append(m_displayUi.showFajrCheckBox->isChecked());
    prayersToShowList.append(m_displayUi.showShroukCheckBox->isChecked());
    prayersToShowList.append(m_displayUi.showDuhrCheckBox->isChecked());
    prayersToShowList.append(m_displayUi.showAsrCheckBox->isChecked());
    prayersToShowList.append(m_displayUi.showMaghribCheckBox->isChecked());
    prayersToShowList.append(m_displayUi.showIshaCheckBox->isChecked());
    
    return prayersToShowList;
}

bool DisplayForm::showNextPrayer()
{
    return m_displayUi.showNextRadioButton->isChecked();
}

bool DisplayForm::showCountdown()
{
    return m_displayUi.countdownRadioButton->isChecked();
}

bool DisplayForm::ignoreHour()
{
    return m_displayUi.ignoreHourCheckBox->isChecked();
}

bool DisplayForm::ignoreSecs()
{
    return m_displayUi.ignoreSecsCheckBox->isChecked();
}

bool DisplayForm::showElapsed()
{
    return m_displayUi.showElapsedCheckBox->isChecked();
}

//bool DisplayForm::iconEffect()
//{
//    return m_displayUi.iconEffectCheckBox->isChecked();
//}

bool DisplayForm::ampmFormat()
{
    return m_displayUi.ampmCheckBox->isChecked();
}

void DisplayForm::setPrayersToShow(QList<bool> prayersToShowList)
{
    m_displayUi.showFajrCheckBox->setChecked(prayersToShowList.at(0));
    m_displayUi.showShroukCheckBox->setChecked(prayersToShowList.at(1));
    m_displayUi.showDuhrCheckBox->setChecked(prayersToShowList.at(2));
    m_displayUi.showAsrCheckBox->setChecked(prayersToShowList.at(3));
    m_displayUi.showMaghribCheckBox->setChecked(prayersToShowList.at(4));
    m_displayUi.showIshaCheckBox->setChecked(prayersToShowList.at(5));
}
/*
void DisplayForm::on_buttonBox_clicked(QAbstractButton *button)
{
    if(m_displayUi.buttonBox->standardButton(button)==QDialogButtonBox::Ok){

        saveSettings();
        emit   accepteChange();
        close();

    }else{
        close();
    }
}
*/
