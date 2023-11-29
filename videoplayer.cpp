#include "videoplayer.h"
#include "ui_videoplayer.h"
#include "Series.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVideoWidget>
#include <MainWindow.h>
#include <qstackedlayout.h>
#include <QLayout>
#include <qpushbutton.h>
#include <QSlider>

VideoPlayer::VideoPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoPlayer)
{
    ui->setupUi(this);
    m_MainWindow = qobject_cast<MainWindow*>(parent);

    m_MediaPlayer = new QMediaPlayer(this);
    m_AudioOutput = new QAudioOutput(this);
    m_VideoWidget = new QVideoWidget(this);

    m_MediaPlayer->setVideoOutput(m_VideoWidget);
    m_MediaPlayer->setAudioOutput(m_AudioOutput);

    m_ControllsWidget = new QWidget(this);
    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setContentsMargins(0, 0, 0, 0);
    m_PlayButton = new QPushButton("Play");
    controlLayout->addWidget(m_PlayButton);
    m_PositionSlider = new QSlider(Qt::Horizontal);
    controlLayout->addWidget(m_PositionSlider);
    m_FullscreenButton = new QPushButton("Fullscreen");
    controlLayout->addWidget(m_FullscreenButton);

    m_VideoPlayerWidget = new QWidget(this);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_VideoWidget);
    layout->addLayout(controlLayout);
    setLayout(layout);


    connect(m_PlayButton,SIGNAL(pressed()),this,SLOT(on_PlayButton_pressed()));
    connect(m_MediaPlayer,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(m_MediaPlayer,SIGNAL(positionChanged(qint64)),this,SLOT(positionChanged(qint64)));
    connect(m_MediaPlayer,SIGNAL(durationChanged(qint64)),this,SLOT(durationChanged(qint64)));
    connect(m_PositionSlider,SIGNAL(sliderMoved(int)),this,SLOT(setPosition(int)));
}

VideoPlayer::~VideoPlayer()
{
    delete ui;
}

void VideoPlayer::SetEpisode(class Episode* newEpisode)
{
    PlayingEpisode = newEpisode;
    m_MediaPlayer->setSource(QUrl::fromLocalFile(newEpisode->getFilePath()));
    m_MediaPlayer->play();
}

void VideoPlayer::on_PlayButton_pressed()
{
    if (m_MediaPlayer->playbackState() == QMediaPlayer::PausedState){
        m_MediaPlayer->play();
    }
    else if (m_MediaPlayer->playbackState() == QMediaPlayer::PlayingState){
        m_MediaPlayer->pause();
    }
}

void VideoPlayer::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status) {
    case QMediaPlayer::NoMedia:{
        qInfo() << "NoMedia";
        break;
    }
    case QMediaPlayer::LoadingMedia:{
        qInfo() << "LoadingMedia";
        break;
    }
    case QMediaPlayer::LoadedMedia:{
        qInfo() << "LoadedMedia";
        break;
    }
    case QMediaPlayer::StalledMedia:{
        qInfo() << "StalledMedia";
        break;
    }
    case QMediaPlayer::BufferingMedia:{
        qInfo() << "BufferingMedia";
        break;
    }
    case QMediaPlayer::BufferedMedia:{
        qInfo() << "BufferedMedia";
        PlayingEpisode->setDuration(m_MediaPlayer->duration());
        PlayingEpisode->UpdateValue();
        break;
    }
    case QMediaPlayer::EndOfMedia:{
        qInfo() << "EndOfMedia";
        break;
    }
    case QMediaPlayer::InvalidMedia:{
        qInfo() << "InvalidMedia";
        break;
    }

    default:
        break;
    }
}

void VideoPlayer::positionChanged(qint64 position)
{
    m_PositionSlider->setSliderPosition(position);
}

void VideoPlayer::durationChanged(qint64 duration)
{
    m_PositionSlider->setRange(0, duration);
}

void VideoPlayer::setPosition(int position)
{
    m_MediaPlayer->setPosition(position);
}
