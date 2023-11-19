
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Library.h>
#include <QScrollArea>
#include <overview.h>
#include <QScrollBar>
#include <AddSeries.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OverviewWidget = new Overview(this);
    LibraryWidget = new Library(this);

    SeriesList = QVector<Series*>{};

    ui->stackedWidget->addWidget(OverviewWidget);
    ui->stackedWidget->addWidget(LibraryWidget);
    ui->stackedWidget->setCurrentWidget(OverviewWidget);

    qInfo() << PROJECT_PATH;
}

MainWindow::~MainWindow()
{
    delete ui;
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
    AddSeries* AddSeriesWidget = new AddSeries(this);

    AddSeriesWidget->exec();
}

