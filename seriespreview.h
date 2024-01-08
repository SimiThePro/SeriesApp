#ifndef SERIESPREVIEW_H
#define SERIESPREVIEW_H

#include <Episode.h>
#include <QWidget>

class SeriesPreview : public QWidget
{
    Q_OBJECT
public:
    explicit SeriesPreview(class Series* series, class MainWindow* mainWindow, QWidget *parent = nullptr);

    void setLastWatchedEpisode(Episode* episode) {m_lastWatchedEpisode = episode;}

    void UpdateProgressBar();
signals:

private slots:
    void on_PlayButton_pressed();
    void on_ExitButtonPressed();

private:
    Series* m_Series;
    Episode* m_lastWatchedEpisode;

    MainWindow* m_MainWindow;

    class QPushButton *PlayButton;
    QPushButton *ExitButton;
    class QProgressBar* Progress;
};

#endif // SERIESPREVIEW_H
