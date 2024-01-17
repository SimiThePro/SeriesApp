#ifndef SERIESPREVIEW_H
#define SERIESPREVIEW_H

#include <QWidget>

class SeriesPreview : public QWidget
{
    Q_OBJECT
public:
    explicit SeriesPreview(class Series* series, class MainWindow* mainWindow, QWidget *parent = nullptr);

    void setLastWatchedEpisode(class Episode* episode);

    void UpdateProgressBar();
    Series* getSeries() const {return m_Series;}
    void setSeries(Series* series) {m_Series = series;}
signals:

private slots:
    void on_PlayButton_pressed();
    void on_ExitButtonPressed();
    void on_BackgroundButton_pressed();

    void mousePressEvent(QMouseEvent *e);

    void enterEvent(QEnterEvent *event); //Hover enter
    void leaveEvent(QEvent* event); //Hover exit
private:
    Series* m_Series;
    Episode* m_lastWatchedEpisode;

    MainWindow* m_MainWindow;

    class QPushButton *PlayButton;
    QPushButton *ExitButton;
    QPushButton* BackgroundButton;
    class QProgressBar* Progress;
};

#endif // SERIESPREVIEW_H
