#include "mediaplayer.h"

MediaPlayer::MediaPlayer(QObject *parent) :
    QObject(parent),mediaPlayer(nullptr)
{
//    connect(&mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
//            this, SLOT(updateState(QMediaPlayer::State)));
//    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)),
//            this, SLOT(updatePosition(qint64)));

}

void MediaPlayer::updatePosition(qint64 pos)
{
  qDebug()<<pos<<mediaPlayer.duration();
  if(pos==mediaPlayer.duration()){
      mediaPlayer.stop();

              emit finished();
  }

}

void MediaPlayer::updateState(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState) {
//        playButton->setToolTip(tr("Pause"));
//        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        emit playingState();
    } else if(state ==QMediaPlayer::PausedState) {
//        playButton->setToolTip(tr("Play"));
//        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
         emit pausedState();
    }else{
           emit stopedState();
    }
}

void MediaPlayer::playMedia(const QString &url)
{

            mediaPlayer.setMedia( QUrl::fromLocalFile(url));
            qDebug()<<"Url"<<QUrl::fromLocalFile(url);
            mediaPlayer.play();
}
void MediaPlayer::stop()
{
    mediaPlayer.stop();
}

bool MediaPlayer::isPlaying()
{
   if(mediaPlayer.state()== QMediaPlayer::PlayingState)
       return true;
   return false;
}

void MediaPlayer::setVolume(int volume)
{
    mediaPlayer.setVolume(volume);
}
int MediaPlayer:: volume()
 {
     return mediaPlayer.volume();
 }
