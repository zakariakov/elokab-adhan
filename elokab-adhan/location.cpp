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


#include "location.h"

//#include <kstandarddirs.h>

#include <QDebug>
#include <QSettings>
LocationForm::LocationForm(QWidget *parent) :
        QWidget(parent)
{
    m_locationUi.setupUi(this);
  this->setAttribute(Qt::WA_DeleteOnClose,true);
    db = new Database;
    QDir appDir(QApplication::applicationDirPath());
    appDir.cdUp();
    m_resourceDir=  appDir.absolutePath()+"/share/elokab/elokab-adhan/data/";

    
    connect(m_locationUi.countriesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCities(int)));
    connect(m_locationUi.citiesComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(updateInfo(const QString&)));
    connect(m_locationUi.manualCalcButton, SIGNAL(clicked()), this, SLOT(manualCalcForm()));
    
    if (!db->checkDatabase())
    {
        qDebug() << "Unable to Read Database File";
        return;
    }

    if (!db->makeConnection())
    {
        qDebug() << "Unable to establish a database connection, this Application needs SQLITE support.";
        return;
    }
    
    QStringList countryCodeList;
    
    QStringList countriesList = db->getCountriesList(&countryCodeList);
    
    for(int i=0;i < countriesList.count();++i)
    {
        QIcon icon(getPixmapForCountryCode(countryCodeList.at(i)));
        QString txt=getTranslation(countryCodeList.at(i),countriesList.at(i));
        m_locationUi.countriesComboBox->addItem(icon,txt, i+1);
    }
    loadSettings();
}
LocationForm::~LocationForm()
{
   db->closeConnection();
}

void LocationForm::saveSettings()
{
    QSettings cg("elokab","adhan");
    cg.beginGroup("Adhan");

    cg.setValue("country", country());
    cg.setValue("city", city());
    cg.setValue("latitude", latitude());
    cg.setValue("longitude", longitude());
    cg.setValue("timezone",timeZone());

    cg.setValue("calcMethod", calcMethod());
    cg.setValue("calcMethodIndex", calcMethodIndex());
    cg.setValue("asrJuristic", asrJuristic());
    cg.setValue("asrJuristicIndex", asrJuristicIndex());
    cg.setValue("highLat", highLat());
    cg.setValue("highLatIndex", highLatIndex());
    cg.setValue("daylight", dayLight());
    cg.setValue("countryNumber", countryNumber());
    cg.setValue("cityNumber", cityNumber());
    cg.setValue("manualCalc", manualCalculation());
    cg.setValue("manualTime", manualTimeList());
    QList<QVariant> fixedTimeList;
    foreach (bool r, m_fixedTimeList) {
              fixedTimeList.append(r);
    }
    cg.setValue("fixedTime", fixedTimeList);




    cg.endGroup();
    //----------------------------------------------------
cg.sync();

}

void LocationForm::loadSettings()
{
    QSettings cg("elokab","adhan");
    cg.beginGroup("Adhan");
    m_locationUi.countriesComboBox->setCurrentIndex(cg.value("countryNumber", 4).toInt());
    m_locationUi.citiesComboBox->setCurrentIndex(cg.value("cityNumber", 3).toInt());
    m_locationUi.longitudeLineEdit->setText(QString::number(cg.value("longitude",3.0597).toDouble()));
    m_locationUi.latitudeLineEdit->setText(QString::number(cg.value("latitude", 36.793).toDouble()));
    m_locationUi.timeZoneSpinBox->setValue(cg.value("timezone", 1.0).toDouble());
    m_locationUi.calcMethodComboBox->setCurrentIndex(cg.value("calcMethodIndex", 0).toInt());
    m_locationUi.asrJuristicComboBox->setCurrentIndex(cg.value("asrJuristicIndex", 0).toInt());
    m_locationUi.highLatComboBox->setCurrentIndex(cg.value("highLatIndex", 1).toInt());
    m_locationUi.dayLightCheckBox->setChecked(cg.value("daylight", false).toBool());
    m_manualCalculation = cg.value("manualCalc", false).toBool();
    m_manualTimeList = cg.value("manualTime", QStringList()).toStringList();





    QList<QVariant> fixedTimeList= cg.value("fixedTime").toList();
    foreach (QVariant r, fixedTimeList) {
        m_fixedTimeList.append(r.toBool());
    }


    cg.endGroup();
    //**********************************

}
 QString LocationForm::getTranslation(const QString & countryCode,const QString & countryName)
 {
     QString lc=this->locale().name().section("_",0,0);
     QString fileName=QString(m_resourceDir+"l10n/%1/entry.desktop").arg(countryCode);
     QSettings s(fileName,QSettings::IniFormat);
     s.beginGroup("KCM Locale");
     s.setIniCodec("UTF-8");
     QString nameOld=s.value("Name",countryName).toString();
     QString name=s.value("Name["+lc+"]",nameOld).toString();
     s.endGroup();
     return name;
 }

QPixmap LocationForm::getPixmapForCountryCode(const QString & countryCode)
{
    if( countryCode.isEmpty() )
        return QPixmap();

    QString flag;
    QPixmap pm = QPixmap();
    
    flag = QString(m_resourceDir+"l10n/%1/flag.png").arg(countryCode);
    if( !flag.isEmpty() )
    {
        pm = QPixmap(flag);
    }
    
    return pm;
}

void LocationForm::updateCities(int index)
{
    m_locationUi.citiesComboBox->clear();
    int countryNo = m_locationUi.countriesComboBox->itemData(index).toInt();
    QStringList citiesList = db->getCitiesList(countryNo);
    
    m_locationUi.citiesComboBox->addItems(citiesList);
}

void LocationForm::updateInfo(QString cityName)
{
    QStringList infoList = db->getInfo(cityName);
    
    if(infoList.size() == 4)
    {
        m_locationUi.latitudeLineEdit->setText(QString::number(infoList.at(0).toDouble()/10000));
        m_locationUi.longitudeLineEdit->setText(QString::number(infoList.at(1).toDouble()/10000));
        m_locationUi.timeZoneSpinBox->setValue(infoList.at(2).toDouble()/100);
        (infoList.at(3).toInt() == 1) ? m_locationUi.dayLightCheckBox->setChecked(true) :
                                       m_locationUi.dayLightCheckBox->setChecked(false);
    }
}

QString LocationForm::country() const
{
    return m_locationUi.countriesComboBox->currentText();
}

QString LocationForm::city() const
{
    return m_locationUi.citiesComboBox->currentText();
}

double LocationForm::longitude() const
{
    return m_locationUi.longitudeLineEdit->text().toDouble();
}

double LocationForm::latitude() const
{
    return m_locationUi.latitudeLineEdit->text().toDouble();
}

double LocationForm::timeZone() const
{
    return m_locationUi.timeZoneSpinBox->value();
}

int LocationForm::countryNumber() const
{
    return m_locationUi.countriesComboBox->currentIndex();
}

int LocationForm::cityNumber() const
{
    return  m_locationUi.citiesComboBox->currentIndex();
}

int LocationForm::calcMethodIndex() const
{
    return m_locationUi.calcMethodComboBox->currentIndex();
}

int LocationForm::calcMethod() const
{
    int calcMethod = PrayerTimes::MWL;
    int index = m_locationUi.calcMethodComboBox->currentIndex();
    
    switch(index)
    {
        case 0:
            calcMethod = PrayerTimes::Makkah;
            break;
        case 1:
            calcMethod = PrayerTimes::Egypt;
            break;         
        case 2:
            calcMethod = PrayerTimes::Karachi;
            break;
        case 3:
            calcMethod = PrayerTimes::ISNA;
            break;
        case 4:
            calcMethod = PrayerTimes::MWL;
            break;
    }
    
    return calcMethod;
}

int LocationForm::asrJuristicIndex() const
{
     return m_locationUi.asrJuristicComboBox->currentIndex();
}

int LocationForm::asrJuristic() const
{
    if(m_locationUi.asrJuristicComboBox->currentIndex() == 0 )
        return PrayerTimes::Shafii;
    else
        return PrayerTimes::Hanafi;
}

int LocationForm::highLatIndex() const
{
     return m_locationUi.highLatComboBox->currentIndex();
}

int LocationForm::highLat() const
{
    int highLat = PrayerTimes::MWL;
    int index = m_locationUi.highLatComboBox->currentIndex();
    
    switch(index)
    {
        case 0:
            highLat = PrayerTimes::None;
            break;
        case 1:
            highLat = PrayerTimes::MidNight;
            break;
        case 2:
            highLat = PrayerTimes::OneSeventh;
            break;
        case 3:
            highLat = PrayerTimes::AngleBased;
            break;
    }
    
    return highLat;
}

bool LocationForm::dayLight() const
{
    return m_locationUi.dayLightCheckBox->isChecked();
}

QStringList LocationForm::manualTimeList() const
{    
    return m_manualTimeList;
}

QList<bool> LocationForm::fixedTimeList() const
{
    return m_fixedTimeList;
}

bool LocationForm::manualCalculation() const
{
    return m_manualCalculation;
}
/*
void LocationForm::setCurrentCountry(int countryNumber)
{
    m_locationUi.countriesComboBox->setCurrentIndex(countryNumber);
}

void LocationForm::setCurrentCity(int cityNumber)
{
    m_locationUi.citiesComboBox->setCurrentIndex(cityNumber);
}

void LocationForm::setLongitude(double longitude)
{
    m_locationUi.longitudeLineEdit->setText(QString::number(longitude));
}

void LocationForm::setLatitude(double latitude)
{
    m_locationUi.latitudeLineEdit->setText(QString::number(latitude));
}

void LocationForm::setTimeZone(double timeZone)
{
    m_locationUi.timeZoneSpinBox->setValue(timeZone);
}

void LocationForm::setCalcMethod(int calcMethodIndex)
{
    m_locationUi.calcMethodComboBox->setCurrentIndex(calcMethodIndex);
}

void LocationForm::setAsrJuristic(int asrJuristicIndex)
{
    m_locationUi.asrJuristicComboBox->setCurrentIndex(asrJuristicIndex);
}

void LocationForm::setHighLat(int highLat)
{
    m_locationUi.highLatComboBox->setCurrentIndex(highLat);
}

void LocationForm::setDayLight(bool dayLight)
{
    m_locationUi.dayLightCheckBox->setChecked(dayLight);
}

void LocationForm::setManualCalculation(bool manualCalc)
{
    m_manualCalculation = manualCalc;
}

void LocationForm::setManualTimeList(QStringList manualTimeList)
{
    m_manualTimeList = manualTimeList;
}

void LocationForm::setFixedTimeList(QList<bool> fixedTimeList)
{
    m_fixedTimeList = fixedTimeList;
}
*/
void LocationForm::manualCalcForm()
{
   ManualCalcForm  *m_manualCalc = new ManualCalcForm;
    m_manualCalc->setManualCalc(m_manualCalculation);
    m_manualCalc->setManualTimeList(m_manualTimeList);
    m_manualCalc->setFixedTimeList(m_fixedTimeList);
    
    if(m_manualCalc->exec())
    {   
        m_manualCalculation = m_manualCalc->manualCalc();
        
        if(m_manualCalculation)
        {     
            m_manualTimeList = m_manualCalc->manualTimeList();
            m_fixedTimeList = m_manualCalc->fixedTimeList();
        }
        
    }
}
/*
void LocationForm::on_buttonBox_clicked(QAbstractButton *button)
{
    if(m_locationUi.buttonBox->standardButton(button)==QDialogButtonBox::Ok){

saveSettings();
              emit   accepteChange();
               close();

    }else{
        close();
    }
}
*/
