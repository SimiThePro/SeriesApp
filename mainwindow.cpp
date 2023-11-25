
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Library.h>
#include <QScrollArea>
#include <overview.h>
#include <QScrollBar>
#include <AddSeries.h>
#include <Series.h>
#include <SeriesOverview.h>
#include <VideoPlayer.h>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OverviewWidget = new Overview(this);
    LibraryWidget = new Library(this);
    m_SeriesOverviewWidget = new SeriesOverview(this);
    m_VideoPlayerWidget = new VideoPlayer(this);

    m_SeriesList = QList<Series*>{};

    ui->stackedWidget->addWidget(OverviewWidget);
    ui->stackedWidget->addWidget(LibraryWidget);
    ui->stackedWidget->addWidget(m_SeriesOverviewWidget);
    ui->stackedWidget->addWidget(m_VideoPlayerWidget);
    ui->stackedWidget->setCurrentWidget(OverviewWidget);

    QFileInfo info("D:/Kurse/Udemy - Unreal Engine 5 - Gameplay Ability System - Top Down RPG/01. Introduction");
    QString st("D:/Kurse/Udemy - Unreal Engine 5 - Gameplay Ability System - Top Down RPG/01. Introduction/01. Introduction.mp4");


    qInfo() << sizeof(info);
    qInfo() << sizeof(st);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddSeries(Series *newSeries)
{
    if (!newSeries) return;

    m_SeriesList.append(newSeries);

    LibraryWidget->AddSeriesToLayout(newSeries);
    newSeries->SaveToFile();

}

void MainWindow::SeriesPressed(Series *pressedSeries)
{
    pressedSeries->MainWindowParent(this);
    m_SeriesOverviewWidget->SetSeries(pressedSeries);
    ui->stackedWidget->setCurrentWidget(m_SeriesOverviewWidget);
}

void MainWindow::SeriesSelected(const QString &Url)
{

    m_VideoPlayerWidget->SetVideo(Url);
    ui->stackedWidget->setCurrentWidget(m_VideoPlayerWidget);
}

void MainWindow::LoadFromFileAndAddSeries()
{

}



void MainWindow::on_OverviewButton_clicked()
{

    ui->stackedWidget->setCurrentWidget(OverviewWidget);
}


void MainWindow::on_LibraryButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(LibraryWidget);
}


void MainWindow::on_AddButton_pressed()
{
    class AddSeries* AddSeriesWidget = new class AddSeries(this);

    AddSeriesWidget->exec();
}


void MainWindow::on_VideoPlayerButton_pressed()
{
    ui->stackedWidget->setCurrentWidget(m_VideoPlayerWidget);
}

