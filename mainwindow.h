
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



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

private slots:
    void on_OverviewButton_clicked();

    void on_LibraryButton_clicked();

    void on_AddButton_pressed();

private:
    Ui::MainWindow *ui;

    class Overview* OverviewWidget;
    class Library* LibraryWidget;
    class SeriesOverview* m_SeriesOverviewWidget;

    QList<class Series*> m_SeriesList;
};

#endif // MAINWINDOW_H
