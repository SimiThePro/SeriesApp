
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

    class Overview* getOverviewWidget() const {return OverviewWidget;}
    QVector<class Series*> getSeriesList() const {return m_SeriesList;}

    void AddSeries(Series* newSeries);
    void SeriesPressed(Series* pressedSeries);
    void EpisodeSelected(class Episode* episode);
    void LoadFromFileAndAddSeries();

    void AddToJSON(QJsonObject newObject);
    void UpdateJSON();
    QJsonArray& getSeriesJsonArray() {return SeriesJsonArray;}
private slots:
    void on_OverviewButton_clicked();

    void on_LibraryButton_clicked();

    void on_AddButton_pressed();

    void on_VideoPlayerButton_pressed();

private:
    Ui::MainWindow *ui;

    class Overview* OverviewWidget;
    class Library* LibraryWidget;
    class SeriesOverview* m_SeriesOverviewWidget;
    class VideoPlayer* m_VideoPlayerWidget;

    QList<class Series*> m_SeriesList;

    QJsonArray SeriesJsonArray;
};

#endif // MAINWINDOW_H
