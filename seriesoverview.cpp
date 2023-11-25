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
        for (const QFileInfo& e : s->getFileList()){

            /*
            QMediaPlayer mediaPlayer;
            mediaPlayer.setSource(QUrl::fromLocalFile(e.absoluteFilePath()));

            QObject::connect(&mediaPlayer, &QMediaPlayer::mediaStatusChanged, [&](QMediaPlayer::MediaStatus status) {
                if (status == QMediaPlayer::LoadedMedia) {
                    QVariant metaData = mediaPlayer.metaData().value(QMediaMetaData::ThumbnailImage);
                    if (metaData.isValid()) {
                        QLabel* ImageThumbnail = new QLabel;
                        QPixmap thumbnail = qvariant_cast<QPixmap>(metaData);
                        ImageThumbnail->setPixmap(thumbnail);
                        // Do something with the thumbnail (e.g., display it)
                    } else {
                        qDebug() << "Thumbnail not available.";
                    }
                }
            });
            */


            QPushButton* btn = new QPushButton(this);
            btn->setText(e.completeBaseName());
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

