#include "seriesoverview.h"
#include <QButtonGroup>
#include <QFileInfo>
#include <QPushButton>
#include <Series.h>
#include <ui_seriesoverview.h>



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
        for (const QFileInfo& e : s->getFileList()){

            QPushButton* btn = new QPushButton(this);
            btn->setText(e.completeBaseName());
            btngroup->addButton(btn);
            layout->addWidget(btn);
        }
        s->SetButtonGroup(btngroup);
        ui->stackedWidget->addWidget(page);
    }
}

void SeriesOverview::on_Sections_currentIndexChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

