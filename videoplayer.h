#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>

namespace Ui {
class VideoPlayer;
}

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();


    void SetVideo(const QString& FileName);
private slots:
    void on_PlayButton_pressed();

private:
    Ui::VideoPlayer *ui;

    QWidget* m_VideoPlayerWidget;
    QWidget* m_ControllsWidget;

    class QMediaPlayer* m_MediaPlayer;
    class QAudioOutput* m_AudioOutput;
    class QVideoWidget* m_VideoWidget;
    class MainWindow* m_MainWindow;

    class QStackedLayout* m_StackedLayout;


    class QPushButton* m_PlayButton;
    class QSlider* m_PositionSlider;
    QPushButton* m_FullscreenButton;
};

#endif // VIDEOPLAYER_H
