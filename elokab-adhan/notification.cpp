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



#include "notification.h"


//#include <kstandarddirs.h>
//#include <KMessageBox>
//#include <QDebug>

//#include <phonon/backendcapabilities.h>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QSlider>
#include <QStyle>
NotificationForm::NotificationForm(QWidget *parent) :
        QWidget(parent)
{
    m_notificationUi.setupUi(this);
   this->setAttribute(Qt::WA_DeleteOnClose,true);
    QDir appDir(QApplication::applicationDirPath());
    appDir.cdUp();
    m_resourceDir=  appDir.absolutePath()+"/share/elokab/elokab-adhan/data/";
//#ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    mediaPlayer=new MediaPlayer;

    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 100);
    connect(slider, SIGNAL(valueChanged(int)), mediaPlayer, SLOT(setVolume(int)));

    slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
       m_notificationUi.horizontalLayout_2->addWidget(slider);
 #else
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    
    Phonon::createPath(mediaObject, audioOutput);
    
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    
    connect(mediaObject, SIGNAL(finished()), this, SLOT(soundFinished()));
    
    volumeSlider = new Phonon::VolumeSlider(this);
    volumeSlider->setAudioOutput(audioOutput);
    volumeSlider->setMuteVisible(false);
    volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    
    m_notificationUi.horizontalLayout_2->addWidget(volumeSlider);
    
      connect(m_notificationUi.enableSoundCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableVolumeSlider(bool)));

#endif
 //#endif
 loadSettings();



      m_notificationUi.playStopToolButton->setIcon(QIcon::fromTheme("media-playback-start",
                                                                    style()->standardIcon(QStyle::SP_MediaPlay)) );

      m_notificationUi.playStopFajrToolButton->setIcon(QIcon::fromTheme("media-playback-start",
                                                                        style()->standardIcon(QStyle::SP_MediaPlay)));
      m_notificationUi.playStopDuaaToolButton->setIcon(QIcon::fromTheme("media-playback-start",
                                                                        style()->standardIcon(QStyle::SP_MediaPlay)));

      connect(m_notificationUi.playStopToolButton, SIGNAL(clicked()), this, SLOT(playStop()));
      connect(m_notificationUi.playStopFajrToolButton, SIGNAL(clicked()), this, SLOT(playStop()));
      connect(m_notificationUi.playStopDuaaToolButton, SIGNAL(clicked()), this, SLOT(playStop()));

      connect(m_notificationUi.tBSelectSound, SIGNAL(clicked()), this, SLOT(checkSoundFile()));
      connect(m_notificationUi.tBSelectSoundFajr, SIGNAL(clicked()), this, SLOT(checkSoundFileFajr()));
      connect(m_notificationUi.tBSelectSoundDuaa, SIGNAL(clicked()), this, SLOT(checkSoundFileDuaa()));



}
NotificationForm::~NotificationForm()
{
 //   #ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
delete mediaPlayer;
#endif
// #endif
}

void NotificationForm::loadSettings()
{
    QSettings cg("elokab","adhan");
    cg.beginGroup("Adhan");

    m_notificationUi.soundSelect->setText(cg.value("soundFilePath", m_resourceDir + "hayy.ogg").toString());
    m_notificationUi.soundFajrSelect->setText(cg.value("soundFajrFilePath", m_resourceDir + "hayy.ogg").toString());
     m_notificationUi.soundDuaaSelect->setText(cg.value("soundDuaaFilePath", m_resourceDir + "duaa.ogg").toString());
    m_notificationUi.enableSoundCheckBox->setChecked(cg.value("athanSoundEnabled", true).toBool());
    m_notificationUi.enableDuaaCheckBox->setChecked(cg.value("duaaSoundEnabled", false).toBool());

//#ifdef USE_MEDIA {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

    mediaPlayer->setVolume(cg.value("soundVolume", 100).toInt());
     slider->setValue(mediaPlayer->volume());
#else
  audioOutput->setVolume(cg.value("soundVolume", 1.0).toReal());
#endif
 //#endif
     m_notificationUi.notifyBeforeCheckBox->setChecked(cg.value("notifyBeforeAthan", true).toBool());
     m_notificationUi.notifyBeforeSpinBox->setValue(cg.value("notifyBeforeAthanVal", 5).toInt());
    m_notificationUi.notifyAfterCheckBox->setChecked(cg.value("notifyAfterAthan", false).toBool());
     m_notificationUi.notifyAfterSpinBox->setValue(cg.value("notifyAfterAthanVal", 5).toInt());

//     m_notificationUi.adhkarNotifyCheckBox->setChecked( cg.value("notifyAdhkar", true).toBool());
//     m_notificationUi.adhkarNotifySpinBox->setValue(cg.value("notifyAdhkarVal", 30).toInt());
//     m_notificationUi.adhkarOrderCheckBox->setChecked( cg.value("orderAdhkar", false).toBool());

     cg.endGroup();
}
void NotificationForm::saveSettings()
{
    QSettings cg("elokab","adhan");
    cg.beginGroup("Adhan");



    cg.setValue("soundFilePath", soundFilePath());
    cg.setValue("soundFajrFilePath", soundFajrFilePath());
    cg.setValue("soundDuaaFilePath", soundDuaaFilePath());
    cg.setValue("athanSoundEnabled", athanSoundEnabled());
    cg.setValue("duaaSoundEnabled", duaaSoundEnabled());
    cg.setValue("soundVolume", soundVolume());
    cg.setValue("notifyBeforeAthan", notifyBeforeAthan());
    cg.setValue("notifyBeforeAthanVal", notifyBeforeAthanVal());
    cg.setValue("notifyAfterAthan", notifyAfterAthan());
    cg.setValue("notifyAfterAthanVal", notifyAfterAthanVal());


//    cg.setValue("notifyAdhkar",  m_notificationUi.adhkarNotifyCheckBox->isChecked());
//    cg.setValue("notifyAdhkarVal", m_notificationUi.adhkarNotifySpinBox->value());
//    cg.setValue("orderAdhkar",m_notificationUi.adhkarOrderCheckBox->isChecked());

  cg.endGroup();
}
void NotificationForm::checkSoundFile()
{
          QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                          soundFilePath(),
                                                           tr("Sound fFiles (*.ogg *.mp3 *.wav);;All files (*.*)"));
                             if(!fileName.isEmpty())
                             m_notificationUi.soundSelect->setText(fileName);
}
void NotificationForm::checkSoundFileFajr()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    soundFajrFilePath(),
                                                     tr("Sound fFiles (*.ogg *.mp3 *.wav);;All files (*.*)"));
                       if(!fileName.isEmpty())
                       m_notificationUi.soundFajrSelect->setText(fileName);
}
void NotificationForm::checkSoundFileDuaa()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     soundDuaaFilePath(),
                                                     tr("Sound fFiles (*.ogg *.mp3 *.wav);;All files (*.*)"));
                       if(!fileName.isEmpty())
                       m_notificationUi.soundDuaaSelect->setText(fileName);
}
// Check if the fileformat is supported by Phonon
//bool NotificationForm::checkSoundFile( const KUrl &url )
//{
//    KUrlRequester *urlSelector = qobject_cast<KUrlRequester *>(sender());

//    if(Phonon::BackendCapabilities::availableMimeTypes().contains(KMimeType::findByUrl(url.url())->name()))
//    {
//        return true;
//    }

//    KMessageBox::sorry(this, i18n("The selected filetype is not supported by Phonon."), i18n("Unsupported filetype"));
    
//    if(urlSelector == m_notificationUi.soundSelect)
//    {
//        m_notificationUi.soundSelect->setUrl(KUrl(KGlobal::dirs()->findResourceDir("data", "mawakit/") + "mawakit/hayy.ogg"));
//        mediaObject->setCurrentSource(m_notificationUi.soundSelect->url());
//    }
//    else if(urlSelector == m_notificationUi.soundFajrSelect)
//    {
//        m_notificationUi.soundFajrSelect->setUrl(KUrl(KGlobal::dirs()->findResourceDir("data", "mawakit/") + "mawakit/hayy.ogg"));
//        mediaObject->setCurrentSource(m_notificationUi.soundFajrSelect->url());
//    }
//    else if(urlSelector == m_notificationUi.soundDuaaSelect)
//    {
//        m_notificationUi.soundDuaaSelect->setUrl(KUrl(KGlobal::dirs()->findResourceDir("data", "mawakit/") + "mawakit/duaa.ogg"));
//        mediaObject->setCurrentSource(m_notificationUi.soundDuaaSelect->url());
//    }
    
//    return false;
//}
//#ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

#else
void NotificationForm::stateChanged(Phonon::State newState, Phonon::State /* oldState */)
{

    switch (newState)
    {
        case Phonon::ErrorState:
            if (mediaObject->errorType() == Phonon::FatalError)
            {
               QMessageBox::critical(this, tr("Fatal Error"), mediaObject->errorString());
            }
            else
            {
             QMessageBox::critical(this, tr("Error"), mediaObject->errorString());
            }
            break;
        case Phonon::PlayingState:
            break;
        case Phonon::StoppedState:
            break;
        case Phonon::PausedState:
            break;
        case Phonon::BufferingState:
            break;
        default:
        ;
    }

}
#endif
//#endif

void NotificationForm::playStop()
{
       QToolButton *toolBtn = qobject_cast<QToolButton *>(sender());
//#ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

    if(mediaPlayer->isPlaying()){
        mediaPlayer->stop();
        toolBtn->setIcon(QIcon::fromTheme("media-playback-start",
                                          style()->standardIcon(QStyle::SP_MediaPlay)));
        tmpToolBtn->setIcon(QIcon::fromTheme("media-playback-start"
                                             ,style()->standardIcon(QStyle::SP_MediaPlay)));
        return;
    }


      if(toolBtn == m_notificationUi.playStopToolButton)
      {
          qDebug()<<m_notificationUi.soundSelect->text();
          mediaPlayer->playMedia(m_notificationUi.soundSelect->text());
      }
      else if(toolBtn == m_notificationUi.playStopFajrToolButton)
      {
            mediaPlayer->playMedia(m_notificationUi.soundFajrSelect->text());
      }
      else if(toolBtn == m_notificationUi.playStopDuaaToolButton)
      {
            mediaPlayer->playMedia(m_notificationUi.soundDuaaSelect->text());
      }
      toolBtn->setIcon(QIcon::fromTheme("media-playback-stop",style()->standardIcon(QStyle::SP_MediaStop)));
      tmpToolBtn = toolBtn;
#else

    
    // If the state is playing, stop the sound media and reset the icon
    if( mediaObject->state() == Phonon::PlayingState )
    {
        mediaObject->stop();
        toolBtn->setIcon(QIcon::fromTheme("media-playback-start",
                                          style()->standardIcon(QStyle::SP_MediaPlay)));
        tmpToolBtn->setIcon(QIcon::fromTheme("media-playback-start",style()->standardIcon(QStyle::SP_MediaPlay)));
        return;
    }
    
    if(toolBtn == m_notificationUi.playStopToolButton)
    {
        mediaObject->setCurrentSource(m_notificationUi.soundSelect->text());
    }
    else if(toolBtn == m_notificationUi.playStopFajrToolButton)
    {
        mediaObject->setCurrentSource(m_notificationUi.soundFajrSelect->text());
    }
    else if(toolBtn == m_notificationUi.playStopDuaaToolButton)
    {
        mediaObject->setCurrentSource(m_notificationUi.soundDuaaSelect->text());
    }
    
    // Play the sound and change the icon
    mediaObject->play();
    toolBtn->setIcon(QIcon::fromTheme("media-playback-stop",style()->standardIcon(QStyle::SP_MediaStop)));
    tmpToolBtn = toolBtn;
#endif
// #endif
}

void NotificationForm::soundFinished()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

#else
    tmpToolBtn->setIcon(QIcon::fromTheme("media-playback-start",style()->standardIcon(QStyle::SP_MediaPlay)));
#endif
}

void NotificationForm::enableVolumeSlider(bool enabled)
{
//#ifdef USE_MEDIA
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

#else
    volumeSlider->setEnabled(enabled);
    if(mediaObject->state() == Phonon::PlayingState)
    {
        mediaObject->stop();
        tmpToolBtn->setIcon(QIcon::fromTheme("media-playback-start",style()->standardIcon(QStyle::SP_MediaPlay)));
    }
    
    if(m_notificationUi.enableDuaaCheckBox->isChecked())
    {
        m_notificationUi.soundDuaaSelect->setEnabled(enabled);
        m_notificationUi.playStopDuaaToolButton->setEnabled(enabled);
    }
#endif
//#endif
}

QString NotificationForm::soundFilePath()
{
    return m_notificationUi.soundSelect->text();
}

QString NotificationForm::soundFajrFilePath()
{
    return m_notificationUi.soundFajrSelect->text();
}

QString NotificationForm::soundDuaaFilePath()
{
    return m_notificationUi.soundDuaaSelect->text();
}

bool NotificationForm::athanSoundEnabled()
{
    return m_notificationUi.enableSoundCheckBox->isChecked();
}

bool NotificationForm::duaaSoundEnabled()
{
    return m_notificationUi.enableDuaaCheckBox->isChecked();
}

qreal NotificationForm::soundVolume()
{
   // #ifdef USE_MEDIA
   #if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  return mediaPlayer->volume();
   #else
      return audioOutput->volume();
   #endif
 //  #endif
}

bool NotificationForm::notifyBeforeAthan()
{
    return m_notificationUi.notifyBeforeCheckBox->isChecked();
}

int NotificationForm::notifyBeforeAthanVal()
{
    return m_notificationUi.notifyBeforeSpinBox->value();
}

bool NotificationForm::notifyAfterAthan()
{
    return m_notificationUi.notifyAfterCheckBox->isChecked();
}

int NotificationForm::notifyAfterAthanVal()
{
    return m_notificationUi.notifyAfterSpinBox->value();
}


/*
void NotificationForm::setSoundFilePath(QString path)
{
    m_notificationUi.soundSelect->setText(path);
}

void NotificationForm::setSoundFajrFilePath(QString path)
{
    m_notificationUi.soundFajrSelect->setText(path);
}

void NotificationForm::setSoundDuaaFilePath(QString path)
{
    m_notificationUi.soundDuaaSelect->setText(path);
}

void NotificationForm::setAthanSoundEnabled(bool enabled)
{
    m_notificationUi.enableSoundCheckBox->setChecked(enabled);
}

void NotificationForm::setDuaaSoundEnabled(bool enabled)
{
    m_notificationUi.enableDuaaCheckBox->setChecked(enabled);
}

void NotificationForm::setSoundVolume(qreal vol)
{
    audioOutput->setVolume(vol);
}

void NotificationForm::setNotifyBeforeAthan(bool enabled)
{
    m_notificationUi.notifyBeforeCheckBox->setChecked(enabled);
}

void NotificationForm::setNotifyBeforeAthanVal(int val)
{
    m_notificationUi.notifyBeforeSpinBox->setValue(val);
}

void NotificationForm::setNotifyAfterAthan(bool enabled)
{
    m_notificationUi.notifyAfterCheckBox->setChecked(enabled);
}

void NotificationForm::setNotifyAfterAthanVal(int val)
{
    m_notificationUi.notifyAfterSpinBox->setValue(val);
}

void NotificationForm::setStopPlayers(bool enabled)
{
    m_notificationUi.stopPlayersCheckBox->setChecked(enabled);
}

void NotificationForm::on_buttonBox_clicked(QAbstractButton *button)
{
          if(m_notificationUi.buttonBox->standardButton(button)==QDialogButtonBox::Ok){

saveSettings();
                    emit   accepteChange();
                     close();

          }else{
              close();
          }
}
*/
