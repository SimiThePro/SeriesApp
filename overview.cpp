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
}

void Overview::on_PlayButton_pressed()
{

}
