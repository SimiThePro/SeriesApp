#include "overview.h"
#include "mainwindow.h"
#include "ui_overview.h"

#include <QProgressBar>
#include <QStackedLayout>
#include <Series.h>
#include <SeriesPreview.h>
#include <qpushbutton.h>

Overview::Overview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Overview)
{
    ui->setupUi(this);
    ui->verticalLayout->setStretch(0,1);
    ui->verticalLayout->setStretch(1,1);
    ui->verticalLayout->setStretch(2,5);

    m_MainWindow = qobject_cast<MainWindow*>(parent);


}

Overview::~Overview()
{
    delete ui;
}

void Overview::AddSeriesToLayout(Series* series)
{
    SeriesPreview* sp = new SeriesPreview(series,m_MainWindow,this);
    ui->Series->addWidget(sp,0,Qt::AlignLeft);
    m_MainWindow->AddToSeriesPreview(sp);
}

void Overview::SetSeriesPreviewPositionToFront(SeriesPreview *overview)
{

    int index = ui->Series->indexOf(overview);
    if (index == -1){
        AddSeriesToLayout(overview->getSeries());
        index = ui->Series->indexOf(overview);
    } 
    ui->Series->insertWidget(0,overview,0,Qt::AlignLeft);
}

void Overview::on_PlayButton_pressed()
{

}
