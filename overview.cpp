#include "overview.h"
#include "ui_overview.h"

#include <Series.h>

Overview::Overview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Overview)
{
    ui->setupUi(this);

    Series* series = new Series(this);
    series->setSeriesName("RPG Top Down");
    series->setSeriesPath(QUrl::fromLocalFile("D:/Kurse/Udemy - Unreal Engine 5 - Gameplay Ability System - Top Down RPG"));
    series->setSeriesIcon(QPixmap(static_cast<QString>(PROJECT_PATH) + "Data/Icons/UdemyCourse"));



    ui->Series->addWidget(series);
}

Overview::~Overview()
{
    delete ui;
}
