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

#include "adhan.h"
#include "adhkarform.h"
#include "ui_adhan.h"

#include "dialogconfig.h"
#include <QCloseEvent>
#include <QDebug>
#include <QSettings>
#include <QGraphicsDropShadowEffect>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
//
#else
#include <QX11Info>
#endif

#include <QDesktopWidget>

#ifdef Q_OS_WIN32
#include <QtWinExtras>
#else
//#include <QX11Info>
#include "notificationutill.h"
#endif

Adhan::Adhan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Adhan)
{
    ui->setupUi(this);
//QFont font;
//font.setFamily("Arial");
//ui->labelInfo->setFont(font);
//ui->labelTimer->setFont(font);
//ui->label->setFont(font);
#ifdef Q_OS_UNIX
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {

        NotificationUtill::Notify("adhan","elokab-adhan",tr("El Adhan"),
                                  tr("I couldn't detect any system tray on this system."),5000);

    }
#endif

    db=new DataBaseAdhkar;
#ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    mediaPlayer=new MediaPlayer;
#endif
#endif
    ui->toolButtonClose->setIcon(QIcon(":/images/cancel"));
    createActions();
    createTrayIcon();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();

    m_today = QDate::currentDate();
    QDir appDir(QApplication::applicationDirPath());

#ifdef Q_OS_HAIKU
    appDir.cd(".");
    m_resourceDir=  appDir.absolutePath()+"/data/";
#else
    appDir.cdUp();
    m_resourceDir=  appDir.absolutePath()+"/share/elokab/elokab-adhan/data/";
#endif


#ifdef Q_OS_UNIX
    QGraphicsDropShadowEffect *fx = new QGraphicsDropShadowEffect();
    if(this->isRightToLeft())
        fx->setOffset(-3,3);
    else
        fx->setOffset(3,3);

    fx->setBlurRadius(10);
    fx->setColor(QColor(63, 63, 63, 200));

    setGraphicsEffect(fx);
#endif


    init();
    //qDebug()<<getenv("DESKTOP_SESSION");

}

Adhan::~Adhan()
{
    delete ui;
}
void Adhan::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {

        hide();
        event->ignore();
    }
}
//! [2]
void Adhan::init()
{
    if (!db->checkDatabase())
    {
        qDebug() << "Unable to Read Database File adhkar no available";
        m_hasAdhkar=false;
    }else{
        m_hasAdhkar=true;
    }

    if (!db->makeConnection())
    {
        qDebug() << "Unable to establish a database connection, this Application needs SQLITE support.adhkar no available";
        m_hasAdhkar=false;
    }else{
        m_hasAdhkar=true;
    }
    m_lastTimeAdhkar=0;
    //---------------------------------------------------------------------------------------
    m_time = QTime::currentTime();
    m_prayerNameList << tr("Fajr") << tr("Sunrise") << tr("Dhuhr")
                     << tr("Asr") << tr("Maghrib") << tr("Isha'a");
    for(int i=0; i<6; i++)
    {
        m_prayerTimeList << QTime(0, 0, 0);
        m_prayerColorList << m_ColorText.name();
    }
    readConfig();
    calculatePrayerTime();
    calculateManualTime();
    m_lastTimeSeen = QTime();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(dataUpdat()));
    timer->start(1000);
    toolTipAboutToShow();

}
void Adhan::dataUpdat()
{


    m_time = QTime::currentTime();

    if (m_time.minute() == m_lastTimeSeen.minute() &&
            m_time.second() == m_lastTimeSeen.second()) {
        // avoid unnecessary repaints
        return;
    }

    m_remainingTime = QTime(0, 0, 0);
    m_remainingTime = m_remainingTime.addSecs(m_time.secsTo(m_nextPrayerTime)+1);

    if(m_remainingTime == QTime(0, 0, 0))
    {
        QString msg = QString(tr("It is time for %1 prayer.")).arg( m_nextPrayerName);

#ifdef Q_OS_UNIX
        if(NotificationUtill::dbusFreedesktopIsValid()){
            NotificationUtill::Notify(this->objectName(),
                                      "elokab-adhan",
                                      tr("Prayer Time"),msg,
                                      50000);
        }else
            trayIcon->showMessage(tr("Prayer Time"),msg);
#else
        trayIcon->showMessage(tr("Prayer Time"),msg);
#endif

        if(m_athanSoundEnabled)
        {
#ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

            if(m_nextPrayerTime == m_prayerTimeList.at(0)) // Fajr adhan
            {
                mediaPlayer->playMedia(m_soundFajrFilePath);
            }
            else
            {
                mediaPlayer->playMedia(m_soundFilePath);
            }
            connect(mediaPlayer, SIGNAL(finished()), this, SLOT(slotSoundFiniched()));

#else

            player = new Phonon::MediaObject(this);
            if(m_nextPrayerTime == m_prayerTimeList.at(0)) // Fajr adhan
            {
                player->setCurrentSource(Phonon::MediaSource(m_soundFajrFilePath));
            }
            else
            {
                player->setCurrentSource(Phonon::MediaSource(m_soundFilePath));
            }

            audioOutput = new Phonon::AudioOutput(this);
            Phonon::createPath(player, audioOutput);
            audioOutput->setVolume(m_soundVolume);

            connect(player, SIGNAL(finished()), this, SLOT(slotSoundFiniched()));
            player->play();
#endif
#endif
        }

        nextPrayer();

        // notification->sendEvent();
    }



    if(m_notifyBeforeAthan)
    {
        if(m_remainingTime.addSecs(-m_notifyBeforeAthanVal*60) == QTime(0, 0, 0))
        {
            QString msg = QString(tr("%1 minute until %2 prayer.", "%1 minutes until %2 prayer."))
                    .arg( m_notifyBeforeAthanVal).arg( m_nextPrayerName);

#ifdef Q_OS_UNIX
            if(NotificationUtill::dbusFreedesktopIsValid()){
                NotificationUtill::Notify(this->objectName(),
                                          "elokab-adhan",tr("Prayer Time"),msg,   50000);


            }else
                trayIcon->showMessage(tr("Prayer Time"),msg);
#else
            trayIcon->showMessage(tr("Prayer Time"),msg);
#endif
        }
    }
    if(m_notifyAfterAthan)
    {
        if(m_previousPrayerTime.secsTo(m_time) == m_notifyAfterAthanVal*60)
        {
            QString msg = QString(tr("%1 minute after %2 prayer.", "%1 minutes after %2 prayer."))
                    .arg( m_notifyAfterAthanVal).arg( m_previousPrayerName);

#ifdef Q_OS_UNIX
            if(NotificationUtill::dbusFreedesktopIsValid()){
                NotificationUtill::Notify(this->objectName(),
                                          "elokab-adhan",tr("Prayer Time"),msg,50000);

            }else
                trayIcon->showMessage(tr("Prayer Time"),msg);
#else
            trayIcon->showMessage(tr("Prayer Time"),msg);
#endif

        }
    }
    if(m_today != QDate::currentDate())
    {
        m_today = QDate::currentDate();
        calculatePrayerTime();
        calculateManualTime();
    }

    //    if(m_iconEffect)
    //        paintIcon();

    // if the tooltip is visible, keep the displayed data up-to-date
    if(this->isVisible())
        toolTipAboutToShow();


    m_lastTimeSeen = m_time;
    //    ADhkar();
    if(m_notifyAdhkar&&m_hasAdhkar){
        m_lastTimeAdhkar++;
        if(m_lastTimeAdhkar==m_notifyAdhkarVal*60){
            showCurentDhikr();
            m_lastTimeAdhkar=0;
        }

    }

}

void Adhan::slotSoundFiniched()
{
#ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if(m_duaaSoundEnabled)
    {
        mediaPlayer->disconnect();
        mediaPlayer->playMedia(m_soundDuaaFilePath);


    }
#else
    if(m_duaaSoundEnabled)
    {
        player->disconnect();
        player->setCurrentSource(Phonon::MediaSource(m_soundDuaaFilePath));
        player->play();
    }
#endif
 #endif
}

void Adhan::readConfig()
{


    QSettings cg("elokab","adhan");
    cg.sync();
    cg.beginGroup("Adhan");

    m_country = cg.value("country", "Algeria").toString();
    m_city = cg.value("city", "Algiers").toString();
    m_latitude = cg.value("latitude", 36.7728).toDouble();
    m_longitude = cg.value("longitude", 3.0603).toDouble();
    m_timeZone = cg.value("timezone", 1.0).toDouble();
    m_calcMethod = cg.value("calcMethod", static_cast<int>(PrayerTimes::Makkah)).toInt();
    m_calcMethodIndex = cg.value("calcMethodIndex", 0).toInt();
    m_asrJuristic = cg.value("asrJuristic", static_cast<int>(PrayerTimes::Shafii)).toInt();
    m_asrJuristicIndex = cg.value("asrJuristicIndex", 0).toInt();
    m_highLat = cg.value("highLat", static_cast<int>(PrayerTimes::MidNight)).toInt();
    m_highLatIndex = cg.value("highLatIndex", 1).toInt();
    m_dayLight = cg.value("daylight", false).toBool();
    m_countryNumber = cg.value("countryNumber", 198).toInt();
    m_cityNumber = cg.value("cityNumber", 91).toInt();
    m_manualCalc = cg.value("manualCalc", false).toBool();

    if(m_manualCalc)
    {
        m_timeDiffList=cg.value("timeDiff").toList();

        m_fixedTimeList = cg.value("fixedTime").toList();
        m_manualTimeList = cg.value("manualTime", QStringList()).toStringList();
    }

    m_soundFilePath = cg.value("soundFilePath", m_resourceDir + "hayy.ogg").toString();
    m_soundFajrFilePath = cg.value("soundFajrFilePath", m_resourceDir + "hayy.ogg").toString();
    m_soundDuaaFilePath = cg.value("soundDuaaFilePath", m_resourceDir + "duaa.ogg").toString();
    m_athanSoundEnabled = cg.value("athanSoundEnabled", true).toBool();
    m_duaaSoundEnabled = cg.value("duaaSoundEnabled", false).toBool();
    m_soundVolume = cg.value("soundVolume", 1.0).toReal();
    m_notifyBeforeAthan = cg.value("notifyBeforeAthan", true).toBool();
    m_notifyBeforeAthanVal = cg.value("notifyBeforeAthanVal", 5).toInt();
    m_notifyAfterAthan = cg.value("notifyAfterAthan", false).toBool();
    m_notifyAfterAthanVal = cg.value("notifyAfterAthanVal", 5).toInt();

    m_notifyAdhkar = cg.value("notifyAdhkar", true).toBool();
    m_notifyAdhkarVal=cg.value("notifyAdhkarVal", 30).toInt();
    m_orderAdhkar= cg.value("orderAdhkar", false).toBool();

    m_prayersToShowList = cg.value("prayersToShowList", QList<QVariant>() << true << false << true
                                   << true << true << true).toList();
    m_showNextPrayer = cg.value("showNextPrayer", false).toBool();
    m_showCountdown = cg.value("showCountdown", true).toBool();
    m_showElapsed = cg.value("showElapsed", true).toBool();
    m_ignoreHour = cg.value("ignoreHour", true).toBool();
    m_ignoreSecs = cg.value("ignoreSecs", true).toBool();
    //   m_iconEffect = cg.value("iconEffect", false).toBool();
    m_ampmFormat = cg.value("ampmFormat", false).toBool();
    //----------colors-----------------
    m_ColorBackground=(cg.value("ColorBackground",QColor(250, 255, 212, 255)).value<QColor>());
    m_ColorText=(cg.value("ColorText",QColor(0, 0, 0, 255)).value<QColor>());
    m_ColorNext=(cg.value("ColorNext",QColor(0, 0, 255, 255)).value<QColor>());
    m_ColorPrev=(cg.value("ColorPrev",QColor(255, 0, 0, 255)).value<QColor>());


    cg.endGroup();
    // adhkarAction->setVisible(m_notifyAdhkar);
    stylize();
}
void Adhan::writeConfig()
{
    QSettings cg("elokab","adhan");
    cg.beginGroup("Adhan");

    if(m_manualCalc && (m_manualTimeList.size() > 0 || m_fixedTimeList.size() > 0))
    {
        cg.setValue("fixedTime", m_fixedTimeList);
        cg.setValue("timeDiff", m_timeDiffList);
        cg.setValue("manualTime", m_manualTimeList);
    }


    cg.endGroup();
}

void Adhan::calculatePrayerTime()
{
    double times[PrayerTimes::TimesCount];

    PrayerTimes prayerTimes(static_cast<PrayerTimes::CalculationMethod>(m_calcMethod),
                            static_cast<PrayerTimes::JuristicMethod>(m_asrJuristic),
                            static_cast<PrayerTimes::AdjustingMethod>(m_highLat)
                            );
    prayerTimes.get_prayer_times(QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().day(),
                                 m_latitude, m_longitude, m_timeZone, m_dayLight, times);

    for(int i=0, j=0; i < PrayerTimes::TimesCount; ++i)
    {
        if(i != PrayerTimes::Sunset) // Skip Sunset
        {
            m_prayerTimeList.replace(j, QTime::fromString(QString::fromStdString(PrayerTimes::float_time_to_time24(times[i])), "hh:mm:ss"));
            j++;
        }
    }

    nextPrayer();
}
void Adhan::calculateManualTime()
{
    if(m_manualCalc)
    {


        for(int i=0; i<6; i++)
        {
            if(m_fixedTimeList.at(i).toBool())
                m_prayerTimeList.replace(i, QTime::fromString(m_manualTimeList.at(i), "hh:mm:ss"));

            else
            {
                m_prayerTimeList.replace(i, m_prayerTimeList.at(i).addSecs(m_timeDiffList.at(i).toInt()));
                m_manualTimeList.replace(i, m_prayerTimeList.at(i).toString("hh:mm:ss"));
            }
        }

        nextPrayer();
    }
}

void Adhan::nextPrayer()
{
    QTime currentTime = m_time;
    ui->labelSalat->setPixmap(":/images/"+QString::number( 0));
    m_nextPrayerName = m_prayerNameList.at(0);
    m_nextPrayerTime = m_prayerTimeList.at(0);
    m_previousPrayerName = m_prayerNameList.at(5);
    m_previousPrayerTime = m_prayerTimeList.at(5);

    int i;
    for(i=0; i<6; i++)
        m_prayerColorList.replace(i, m_ColorText.name()); // Not prayed

    for(i=0; i<6; i++)
    {
        qDebug()<<i<<currentTime.toString("hh:mm:ss");
        if(currentTime < m_prayerTimeList.at(i) && m_prayersToShowList.at(i).toBool())
        {
            qDebug()<<"curent======"<<i<<m_prayerNameList.at(i);
            ui->labelSalat->setPixmap(":/images/"+QString::number( i));

            m_nextPrayerName = m_prayerNameList.at(i);
            m_nextPrayerTime = m_prayerTimeList.at(i);
            m_prayerColorList.replace(i, m_ColorNext.name()); // Current prayer


            break;
        }
    }

    //    m_prayerNameList << tr("Fajr") << tr("Sunrise") << tr("Dhuhr")
    //                     << tr("Asr") << tr("Maghrib") << tr("Isha'a");

    for(int j=0; j<i; j++)
        m_prayerColorList.replace(j, m_ColorPrev.name()); // Prayed

    if(i)
    {
        if(i!=2)
        {
            m_previousPrayerName = m_prayerNameList.at(i-1);
            m_previousPrayerTime = m_prayerTimeList.at(i-1);
        }

        else // Avoid Sunrise case
        {
            m_previousPrayerName = m_prayerNameList.at(i-2);
            m_previousPrayerTime = m_prayerTimeList.at(i-2);
        }

        if(i==6)
            m_prayerColorList.replace(0,  m_ColorNext.name()); // Current prayer in next day
    }

    for(i=0; i<6; i++)
    {
        qDebug()<<m_prayerNameList.at(i)+" = " +m_prayerTimeList.at(i).toString("hh:mm:ss");
    }


    toolTipAboutToShow();
}
/********************************************
  *********CONFIGURATION*************
  ********************************************/
void Adhan::showConfiguration()
{
    /*
  LocationForm  *m_location=new LocationForm;

    connect(m_location,SIGNAL(accepteChange()),this,SLOT(locationAccepted()));
    m_location->show();*/
    db->closeConnection();
    DialogConfig *dlg=new DialogConfig;
    //connect(dlg,SIGNAL(accepted()),this,SLOT(configurationAccepted()));

    if(dlg->exec()==QDialog::Accepted){
        configurationAccepted();
    }

    if ( m_notifyAdhkar&&m_hasAdhkar)
    {
        db->makeConnection();
    }
}
void Adhan::showCurentDhikr()
{

    //    AdhkarForm *dlg=new AdhkarForm;
    //    dlg->show();
    //    return;



    if(m_notifyAdhkar&&m_hasAdhkar){
        QStringList list=db->getDhikr(m_orderAdhkar);
        qDebug()<<list.count();
        if(list.count()<1)return;
#ifdef Q_OS_UNIX
        if(NotificationUtill::dbusFreedesktopIsValid()){
            NotificationUtill::Notify(this->objectName(),
                                      "elokab-adhan",list.at(0),
                                      list.at(1),
                                      50000);
        }else
            trayIcon->showMessage(list.at(0),list.at(1),QSystemTrayIcon::Information,50000);
#else
        trayIcon->showMessage(list.at(0),list.at(1),QSystemTrayIcon::Information,50000);

#endif

    }

}

void Adhan::configurationAccepted()
{
    readConfig();
    calculatePrayerTime();
    if(m_manualCalc)
    {
        m_timeDiffList.clear();

        for(int i=0; i<6; i++)
        {
            m_timeDiffList.append(m_prayerTimeList.at(i).secsTo(QTime::fromString(m_manualTimeList.at(i), "hh:mm:ss")));
        }
    }

    calculateManualTime();
    writeConfig();
}



/****************************************
  ************toolTip***************
  *************************************/
void Adhan::toolTipAboutToShow()
{
    QString tip="";
    if(isRightToLeft())
        tip ="<html dir='rtl' align='right'>";



    QString clockFormat = "hh:mm";

    if(m_ampmFormat)
        clockFormat = "hh:mm AP";

    QTime elapsedTime = QTime(0, 0, 0);
    elapsedTime = elapsedTime.addSecs(m_previousPrayerTime.secsTo(m_time));

     tip += "<table>";
    for(int i=0; i<6; i++)
    {
        if(m_prayersToShowList.at(i).toBool())
        {
            tip +=QString ("<tr><td><font color='%2'>%1  :%3</font></td><td></td></tr>")
                    .arg( m_prayerNameList.at(i))
                    .arg(m_prayerColorList.at(i))
                    .arg(m_prayerTimeList.at(i).toString(clockFormat));

        }
    }
    tip += ("</table>");

    trayIcon->setToolTip(QString(" %1 : %2 ")
                         .arg(m_nextPrayerName).arg( m_nextPrayerTime.toString(clockFormat)));
    if(m_showNextPrayer)
    {
        ui->labelTimer->setText( QString("%1\n%2").arg( m_nextPrayerTime.toString(clockFormat)).arg(m_nextPrayerName));


    }

    else
    {


        if(m_showElapsed && elapsedTime < QTime(0, 10, 0)) // Show 10 minutes after prayer time
            ui->labelTimer->setText(QString (tr("%1\n from %2")).arg(elapsedTime.toString("mm:ss")).arg(m_previousPrayerName));

        else if(m_showCountdown)
        {
            if(m_ignoreHour && m_remainingTime.hour() == 0)
                ui->labelTimer->setText( QString("%1").arg( m_remainingTime.toString("mm:ss"))+"\n"+QString(tr(" to %1")).arg(m_nextPrayerName));

            else if(m_ignoreSecs && m_remainingTime.hour() > 0)
                ui->labelTimer->setText(QString ("%1").arg( m_remainingTime.toString("hh:mm"))+"\n"+QString(tr(" to %1")).arg(m_nextPrayerName));

            else
                ui->labelTimer->setText( m_remainingTime.toString("hh:mm:ss")+"\n"+QString(tr(" to %1")).arg(m_nextPrayerName));

            // tip +=QString(tr(" to %1")).arg(m_nextPrayerName);

        }
    }
    qDebug()<<m_remainingTime.toString("hh:mm:ss")+QString(tr(" to %1")).arg(m_nextPrayerName);
    ui->labelInfo->setText(tip);
    //ui->labelTimer->setText(m_remainingTime.toString("hh:mm:ss"));
}

/********************************************
  ***************SYSTEM TRAY****************
**************************************************/

void Adhan::createActions()
{


    configAction = new QAction(QIcon::fromTheme("configure",QIcon::fromTheme("document-properties")),tr("&Configure Prayer Times"), this);
    connect(configAction, SIGNAL(triggered()), this, SLOT(showConfiguration()));
    showAction = new QAction(QIcon::fromTheme("documentinfo"),tr("&Prayer Times"), this);
    connect(showAction, SIGNAL(triggered()), this, SLOT(show()));

    adhkarAction = new QAction(QIcon::fromTheme("documentinfo"),tr("&New Dhikr"), this);
    connect(adhkarAction, SIGNAL(triggered()), this, SLOT(showCurentDhikr()));



    quitAction = new QAction(QIcon::fromTheme("application-exit"),tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Adhan::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(adhkarAction);
    trayIconMenu->addAction(showAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(configAction);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon::fromTheme("adhan-app",QIcon(":/images/adhan-app.png")));
}
void Adhan::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        if(isVisible())
            this->hide();
        else
            this->show();

        break;
    case QSystemTrayIcon::MiddleClick:
        showCurentDhikr();
        break;
    default:
        ;
    }
}
void  Adhan::loadSettings(bool composit)
{

    //*********************popup*************************
    QString myStyleCmposite=QString("\n/*syle for main menu background... */"
                                    "\n QLabel{\n  color: %5;\n }"
                                    "\n QWidget #widgetBgrmenu{"
                                    "\n   background-color: rgba(%1, %2, %3,%4);"
                                    "\n   border-radius:10px;"
                                    "\n }"
                                    "\n QToolButton{"
                                    "\n      border-radius:2px;"
                                    "\n      background:transparent;\n }"
                                    "\n QToolButton:hover{"
                                    "\n      border:1px solid %5;\n }"
                                      "\n QLabel #labelInfo{ background-color: rgba(%1, %2, %3,%4); }"
                                    )

            .arg(m_ColorBackground.red())
            .arg(m_ColorBackground.green())
            .arg(m_ColorBackground.blue())
            .arg(m_ColorBackground .alpha())
            .arg(m_ColorText.name());



    QString myStyleNonCmposite=QString("\n/*syle for main menu background... */"
                                       "\n QLabel{\n  color: %5;\n }"
                                       "\n QWidget #widgetBgrmenu{"
                                       "\n   border-radius:0px;"
                                       "\n   background-color: rgba(%1, %2, %3,%4);"
                                       "\n }"
                                       "\n QToolButton{"
                                       "\n      border-radius:2px;"
                                       "\n      background:transparent;\n }"
                                       "\n QToolButton:hover{"
                                       "\n      border:1px solid %5;\n }"
                                       )
            .arg(m_ColorBackground.red())
            .arg(m_ColorBackground.green())
            .arg(m_ColorBackground.blue())
            .arg(m_ColorBackground .alpha())
            .arg(m_ColorText.name());
    if(composit){
        ui->gridLayoutAdhan->setContentsMargins(4,4,4,4);
        this->setStyleSheet(myStyleCmposite);

    }else{
        ui->gridLayoutAdhan->setContentsMargins(1,1,1,1);
        this->setStyleSheet(myStyleNonCmposite);

    }



}
void Adhan::show()
{

    //   if (m_isCompositing!=QX11Info::isCompositingManagerRunning()){
    //       m_isCompositing=QX11Info::isCompositingManagerRunning();
    //       qDebug()<<"coposite changed"<<m_isCompositing;
    //         loadSettings(QString());
    //   }
    adjustSize();
    stylize();
    QWidget::show();

    int wP=(size().width()/2);
    int hP=(size().height()/2);

    QPoint  m_pos=(QPoint(QCursor::pos().x()-wP,QCursor::pos().y()-hP));


    QRect screen = QApplication::desktop()->availableGeometry(m_pos);

    QRect rect;


    rect.setSize(this->size());




    rect.moveTo(m_pos);
    if (rect.right() > screen.right())
        rect.moveRight(screen.right()-12);


    if (rect.bottom() > screen.bottom())
        rect.moveBottom(screen.bottom()-12);

    if (rect.left() < screen.left())
        rect.moveLeft(screen.left());

    if (rect.top() < screen.top())
        rect.moveTop(screen.top());
    //qgetenv(key.toUtf8().constData());

    qDebug()<<rect.topLeft()<<rect.bottomLeft()<<screen.bottom();
    qDebug()<<rect;
    move(rect.topLeft());

    //  qDebug()<<m_ColorBackground.red()<<m_ColorBackground.green()<<m_ColorBackground.blue()<<m_ColorBackground.alpha();
}

void Adhan::stylize()
{
#ifdef Q_OS_WIN32

    if (QtWin::isCompositionEnabled()) {
        setWindowFlags(  Qt::Tool |Qt::CustomizeWindowHint| Qt::WindowStaysOnTopHint/* | Qt::CustomizeWindowHint | Qt::X11BypassWindowManagerHint*/);

        QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
        setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_NoSystemBackground, false);
        m_isCompositing=true;
        setStyleSheet("Adhan { background: transparent; }");
       ui->labelInfo->setStyleSheet(QString(" background-color: rgba(%1, %2, %3,%4);")
               .arg(m_ColorBackground.red())
               .arg(m_ColorBackground.green())
               .arg(m_ColorBackground.blue())
               .arg(m_ColorBackground .alpha())
               .arg(m_ColorText.name()));
    } else {
        setWindowFlags(  Qt::Popup |Qt::CustomizeWindowHint| Qt::WindowStaysOnTopHint/* | Qt::CustomizeWindowHint | Qt::X11BypassWindowManagerHint*/);

        QtWin::resetExtendedFrame(this);
        m_isCompositing=false;
        setAttribute(Qt::WA_TranslucentBackground, false);
        //   setStyleSheet(QString("MusicPlayer { background: %1; }").arg(QtWin::realColorizationColor().name()));
        loadSettings(false);
    }
    //volumeButton->stylize();
#else

    #if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    setWindowFlags(  Qt::Popup |Qt::CustomizeWindowHint| Qt::WindowStaysOnTopHint/* | Qt::CustomizeWindowHint | Qt::X11BypassWindowManagerHint*/);

    setAttribute(Qt::WA_TranslucentBackground, false);
    loadSettings(false);
#else
    if(QX11Info::isCompositingManagerRunning()){
        setWindowFlags(  Qt::Popup |Qt::CustomizeWindowHint| Qt::WindowStaysOnTopHint/* | Qt::CustomizeWindowHint | Qt::X11BypassWindowManagerHint*/);

        setAttribute(Qt::WA_TranslucentBackground, true);
        loadSettings(true);
    }
#endif


#endif

}
