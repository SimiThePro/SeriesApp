#include "overview.h"
#include "mainwindow.h"
#include "ui_overview.h"

#include <Series.h>

Overview::Overview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Overview)
{
    ui->setupUi(this);

    m_MainWindow = qobject_cast<MainWindow*>(parent);



}

Overview::~Overview()
{
    delete ui;
}

void Overview::AddSeries(Series *series)
{

    if (!series) return;
    Series* NewSeries = new Series(series->getSeriesPath(),series->getSeriesIconPath(),series->getSeriesName(),this);

    m_MainWindow->getSeriesList().append(NewSeries);
    ui->Series->addWidget(series,0,Qt::AlignLeft);
}
