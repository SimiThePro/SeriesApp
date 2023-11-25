#include "seriesoverview.h"
#include <QButtonGroup>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QPushButton>
#include <QScrollBar>
#include <Series.h>
#include <qscrollarea.h>
#include <ui_seriesoverview.h>
#include <QMediaMetaData>
#include <QDataStream>


SeriesOverview::SeriesOverview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SeriesOverview)
{
    ui->setupUi(this);
}

SeriesOverview::~SeriesOverview()
{
    delete ui;
}

SeriesOverview::SeriesOverview(Series *series, QWidget *parent):
    QWidget(parent),
    ui(new Ui::SeriesOverview)
{
    ui->setupUi(this);
}

void SeriesOverview::SetSeries(Series *series)
{
    ui->Sections->clear();

    for(int i = ui->stackedWidget->count(); i >= 0; i--)
    {
        QWidget* widget = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }

    ui->Title->setText(series->getSeriesName());


    QList<Section*> Sections = series->getSections();

    for (Section* s : Sections){
        QButtonGroup* btngroup = new QButtonGroup(this);
        ui->Sections->addItem(s->getSectionName());
        QWidget* page = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout(page);


        layout->setAlignment(Qt::AlignTop);
        for (Episode* e : s->getEpisodes()){
            QPushButton* btn = new QPushButton(this);
            btn->setText(e->getEpisodeName());
            btngroup->addButton(btn);
            layout->addWidget(btn);
        }

        QScrollArea* SA = new QScrollArea;
        SA->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        SA->setWidget(page);
        s->SetButtonGroup(btngroup);
        ui->stackedWidget->addWidget(SA);
    }
}

void SeriesOverview::on_Sections_currentIndexChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

