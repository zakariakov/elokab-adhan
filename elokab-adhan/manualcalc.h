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


#ifndef MANUALCALC_H
#define MANUALCALC_H

#include <QDialog>

#include "ui_manualcalcform.h"


class ManualCalcForm : public QDialog
{
    Q_OBJECT
public:
    ManualCalcForm(QDialog *parent = 0);
    
    bool manualCalc() const;
    QStringList manualTimeList();
    QList<bool> fixedTimeList();
    
    void setManualCalc(bool enabled);
    void setManualTimeList(QStringList strTime);
    void setFixedTimeList(QList<bool> enabled);
    
private:    
    Ui::ManualCalcForm m_manualCalcUi;
    
    QStringList m_manualTimeList;
    QList<bool> m_fixedTimeList;
};

#endif
