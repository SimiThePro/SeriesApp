#include "videoplayer.h"
#include "ui_videoplayer.h"
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

}

VideoPlayer::~VideoPlayer()
{
    delete ui;
}

void VideoPlayer::SetVideo(const QString &FileName)
{
    m_MediaPlayer->setSource(FileName);
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
