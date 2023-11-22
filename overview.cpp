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

void Overview::AddSeriesToLayout(Series* series)
{
    ui->Series->addWidget(series,0,Qt::AlignLeft);
}
