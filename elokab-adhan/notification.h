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

#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include <QObject>
#include <QWidget>

#ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include "mediaplayer.h"
#else
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include <phonon/seekslider.h>

#include <phonon/volumeslider.h>
#include <phonon/backendcapabilities.h>
#include <phonon/effect.h>
#include <phonon/effectparameter.h>
#include <phonon/effectwidget.h>
#endif
#endif

//#include <Phonon/MediaObject>
//#include <Phonon/VolumeSlider>
//#include <Phonon/MediaObject>

class QSlider;

#include "ui_notificationform.h"

class NotificationForm : public QWidget
{
    Q_OBJECT
public:
    NotificationForm(QWidget *parent =nullptr);
    ~NotificationForm();

    void saveSettings();
    /*
    void setSoundFilePath(QString path);
    void setSoundFajrFilePath(QString path);
    void setSoundDuaaFilePath(QString path);
    void setAthanSoundEnabled(bool enbaled);
    void setDuaaSoundEnabled(bool enbaled);
    void setSoundVolume(qreal vol);
    void setNotifyBeforeAthan(bool enabled);
    void setNotifyBeforeAthanVal(int val);
    void setNotifyAfterAthan(bool enabled);
    void setNotifyAfterAthanVal(int val);
    void setStopPlayers(bool enabled);
    */
signals:
    void  accepteChange();
private slots:
    void loadSettings();

    void checkSoundFile();
    void checkSoundFileFajr();
    void checkSoundFileDuaa();
    void enableVolumeSlider(bool enabled);
#ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

#else
    void stateChanged(Phonon::State newState, Phonon::State oldState);
#endif
#endif
    void playStop();
    void soundFinished();
    
    // void on_buttonBox_clicked(QAbstractButton *button);

private:
#ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    MediaPlayer *mediaPlayer;
#else
    Phonon::VolumeSlider *volumeSlider;
    Phonon::MediaObject *mediaObject;
    Phonon::AudioOutput *audioOutput;
#endif
#endif
    QString  m_resourceDir;
    QToolButton *tmpToolBtn;

    Ui::NotificationForm m_notificationUi;
    
    QString soundFilePath();
    QString soundFajrFilePath();
    QString soundDuaaFilePath();
    bool athanSoundEnabled();
    bool duaaSoundEnabled();
    qreal soundVolume();
    bool notifyBeforeAthan();
    int notifyBeforeAthanVal();
    bool notifyAfterAthan();
    int notifyAfterAthanVal();
    QSlider *slider;
};

#endif
