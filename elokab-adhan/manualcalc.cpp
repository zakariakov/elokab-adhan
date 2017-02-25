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


#include "manualcalc.h"


ManualCalcForm::ManualCalcForm(QDialog *parent) :
        QDialog(parent)
{
    m_manualCalcUi.setupUi(this);
   // this->setAttribute(Qt::WA_DeleteOnClose,true);
}

bool ManualCalcForm::manualCalc() const
{
    return m_manualCalcUi.manualCalcGroupBox->isChecked();
}

QStringList ManualCalcForm::manualTimeList()
{
    m_manualTimeList.append(m_manualCalcUi.fajrTimeEdit->time().toString("hh:mm:ss"));
    m_manualTimeList.append(m_manualCalcUi.shroukTimeEdit->time().toString("hh:mm:ss"));
    m_manualTimeList.append(m_manualCalcUi.duhrTimeEdit->time().toString("hh:mm:ss"));
    m_manualTimeList.append(m_manualCalcUi.asrTimeEdit->time().toString("hh:mm:ss"));
    m_manualTimeList.append(m_manualCalcUi.maghribTimeEdit->time().toString("hh:mm:ss"));
    m_manualTimeList.append(m_manualCalcUi.ishaTimeEdit->time().toString("hh:mm:ss"));
    
    return m_manualTimeList;
}

QList<bool> ManualCalcForm::fixedTimeList()
{
    m_fixedTimeList.append(m_manualCalcUi.fixedFajrCheckBox->isChecked());
    m_fixedTimeList.append(m_manualCalcUi.fixedShroukCheckBox->isChecked());
    m_fixedTimeList.append(m_manualCalcUi.fixedDuhrCheckBox->isChecked());
    m_fixedTimeList.append(m_manualCalcUi.fixedAsrCheckBox->isChecked());
    m_fixedTimeList.append(m_manualCalcUi.fixedMaghribCheckBox->isChecked());
    m_fixedTimeList.append(m_manualCalcUi.fixedIshaCheckBox->isChecked());
    
    return m_fixedTimeList;
}

void ManualCalcForm::setManualCalc(bool enabled)
{
    m_manualCalcUi.manualCalcGroupBox->setChecked(enabled);
}

void ManualCalcForm::setManualTimeList(QStringList strTime)
{
    if(strTime.size() > 5)
    {
        m_manualCalcUi.fajrTimeEdit->setTime(QTime::fromString(strTime.at(0), "hh:mm:ss"));
        m_manualCalcUi.shroukTimeEdit->setTime(QTime::fromString(strTime.at(1), "hh:mm:ss"));
        m_manualCalcUi.duhrTimeEdit->setTime(QTime::fromString(strTime.at(2), "hh:mm:ss"));
        m_manualCalcUi.asrTimeEdit->setTime(QTime::fromString(strTime.at(3), "hh:mm:ss"));
        m_manualCalcUi.maghribTimeEdit->setTime(QTime::fromString(strTime.at(4), "hh:mm:ss"));
        m_manualCalcUi.ishaTimeEdit->setTime(QTime::fromString(strTime.at(5), "hh:mm:ss"));
    }
}

void ManualCalcForm::setFixedTimeList(QList<bool> enabled)
{
    m_manualCalcUi.fixedFajrCheckBox->setChecked(enabled.at(0));
    m_manualCalcUi.fixedShroukCheckBox->setChecked(enabled.at(1));
    m_manualCalcUi.fixedDuhrCheckBox->setChecked(enabled.at(2));
    m_manualCalcUi.fixedAsrCheckBox->setChecked(enabled.at(3));
    m_manualCalcUi.fixedMaghribCheckBox->setChecked(enabled.at(4));
    m_manualCalcUi.fixedIshaCheckBox->setChecked(enabled.at(5));
}
