
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <qjsonarray.h>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    class Overview* OverviewWidget;
    class Library* LibraryWidget;
    class SeriesOverview* m_SeriesOverviewWidget;
    class VideoPlayer* m_VideoPlayerWidget;


public: //functions

    class Overview* getOverviewWidget() const {return OverviewWidget;}
    class SeriesOverview* getSeriesOverviewWidget() const {return m_SeriesOverviewWidget;}
    QVector<class Series*> getSeriesList() const {return m_SeriesList;}

    void AddSeries(Series* newSeries);
    void SeriesPressed(Series* pressedSeries);
    void EpisodeSelected(class Episode* episode);
    void LoadFromFileAndAddSeries();
    void SimpleUpdateSeries(Series* UpdatedSeries); //Updates Icon and Name

    void AddToJSON(QJsonObject newObject);
    void UpdateJSON();
    QJsonArray& getSeriesJsonArray() {return SeriesJsonArray;}
    void setSeriesJsonArray(const QJsonArray& newJsonArray) {SeriesJsonArray = newJsonArray;}

    void AddToPending(Episode* pendingEpisode);

    void MakeVideoPlayerFullscreen();
    void ReturnToSeriesOverviewWidget();

public slots:

private slots:
    void on_OverviewButton_clicked();

    void on_LibraryButton_clicked();

    void on_AddButton_pressed();

    void on_VideoPlayerButton_pressed();

    void on_MPVVideoPlayer_closed(int,QProcess::ExitStatus);
private:
    Ui::MainWindow *ui;

    class QProcess* m_MPVVideoPlayer;

    class Episode* m_currentEpisode;
    QList<class Series*> m_SeriesList;
    QList<class Episode*> m_pendingChangedEpisodes;
    QJsonArray SeriesJsonArray;
};

#endif // MAINWINDOW_H
