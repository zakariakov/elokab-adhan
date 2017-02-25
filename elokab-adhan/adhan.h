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


#ifndef ADHAN_H
#define ADHAN_H

#include <QWidget>
#include "databaseadhkar.h"

#ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include "mediaplayer.h"
#else
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#endif
#endif

#include <QMainWindow>
#include <QDate>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QFileSystemWatcher>
#include <QSettings>
#include <QDir>

namespace Ui {
class Adhan;
}

class Adhan : public QWidget
{
    Q_OBJECT
    
public:
    explicit Adhan(QWidget *parent = 0);
    ~Adhan();
    void show();
protected:
    void closeEvent(QCloseEvent *event);

private slots:


    void init();
    void calculatePrayerTime();
    void calculateManualTime();
    void dataUpdat();
    void slotSoundFiniched();
    void nextPrayer();

    void  readConfig();
    void writeConfig();

    void showConfiguration();
void showCurentDhikr();
    void configurationAccepted();

    void toolTipAboutToShow();
    //sys try
    void createActions();
    void createTrayIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void  loadSettings(bool /*p*/);
   void stylize();
private:
    Ui::Adhan *ui;
    QString m_country;
    QString m_city;
    double m_longitude;
    double m_latitude;
    double m_timeZone;
    int m_calcMethod;
    int m_asrJuristic;
    int m_highLat;
    bool m_dayLight;
    int m_countryNumber;
    int m_cityNumber;
    int m_calcMethodIndex;
    int m_asrJuristicIndex;
    int m_highLatIndex;

    QList<QTime> m_prayerTimeList;
    QStringList m_prayerNameList;

    QList<QVariant> m_timeDiffList;//int
    QStringList m_manualTimeList;
    QList<QVariant> m_fixedTimeList;//bool
    bool m_manualCalc;

    QString m_resourceDir;
    QString m_soundFilePath;
    QString m_soundFajrFilePath;
    QString m_soundDuaaFilePath;
    bool m_athanSoundEnabled;
    bool m_duaaSoundEnabled;
    qreal m_soundVolume;
    bool m_notifyBeforeAthan;
    int m_notifyBeforeAthanVal;
    bool m_notifyAfterAthan;
    int m_notifyAfterAthanVal;


    QList<QVariant> m_prayersToShowList;//bool
    bool m_showNextPrayer;
    bool m_showCountdown;
    bool m_showElapsed;
    bool m_ignoreHour;
    bool m_ignoreSecs;
//    bool m_iconEffect;
    bool m_ampmFormat;

    QStringList m_prayerColorList;

    QString m_nextPrayerName;
    QString m_previousPrayerName;
    QTime m_nextPrayerTime;
    QTime m_previousPrayerTime;
    QTime m_remainingTime;

    QTimer m_timer;
    QTime m_time;
    QTime m_lastTimeSeen;
    QTime m_tmpTime;
    QDate m_today;
#ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  MediaPlayer *mediaPlayer;
#else
    Phonon::MediaObject* player;
    Phonon::AudioOutput* audioOutput;
#endif
  #endif
    QList<QAction *> m_actions;
    QAction *m_qiblaDirectionAct;
    QAction *m_monthlyPrayerAct;
    // LocationForm *m_location;
    //        LocationForm *m_location;
   // NotificationForm *m_notification;
  //  DisplayForm *m_display;
    //----------colors-----------------
   QColor    m_ColorBackground;
   QColor     m_ColorText;
   QColor     m_ColorNext;
   QColor     m_ColorPrev;

    //system try
    QAction *adhkarAction;
    QAction *showAction;
    QAction *configAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
bool m_isCompositing;
//    QSettings *setting;
//    QFileSystemWatcher *m_fileWatcher ;
//adhkar--------------------------------------
    DataBaseAdhkar *db;
    bool m_hasAdhkar;
    bool m_orderAdhkar;
    bool m_notifyAdhkar;
    int m_notifyAdhkarVal;
     int m_lastTimeAdhkar;
};

#endif // ADHAN_H
