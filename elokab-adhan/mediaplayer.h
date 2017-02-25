#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QMediaPlayer>

class MediaPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MediaPlayer(QObject *parent = 0);
void playMedia(const QString &url);
void stop();
bool isPlaying();

int volume();
signals:
void  playingState();
void  pausedState();
void  stopedState();
void finished();

public slots:
void setVolume(int volume);
private:
      QMediaPlayer mediaPlayer;

private slots:
    void  updateState(QMediaPlayer::State state);
    void updatePosition(qint64 pos);
};

#endif // MEDIAPLAYER_H
