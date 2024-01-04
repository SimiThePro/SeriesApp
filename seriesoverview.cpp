#include "seriesoverview.h"
#include "episode.h"
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
#include <QProgressBar>
#include <QFormLayout>
#include <CustomPushButton.h>
#include <MainWindow.h>


SeriesOverview::SeriesOverview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SeriesOverview)
{
    ui->setupUi(this);
    m_MainWindow = qobject_cast<MainWindow*>(parent);
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
    m_MainWindow = qobject_cast<MainWindow*>(parent);
}

void SeriesOverview::SetSeries(Series *series)
{
    ui->Sections->clear();

    Update(series);

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
        page->setLayout(layout); // Set the layout for the page widget

        layout->setAlignment(Qt::AlignTop);
        for (Episode* e : s->getEpisodes()){
            QWidget* innerpage = new QWidget;
            QHBoxLayout* innerlayout = new QHBoxLayout(innerpage);
            CustomPushButton* btn = new CustomPushButton(this,btngroup);
            btn->setText(e->getEpisodeName());
            btngroup->addButton(btn);

            // Set the horizontal stretch for the button and progress bar
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

            innerlayout->addWidget(btn, 1); // Allocate 50% of space

            if (e->getProgress() != -1 && e->getProgress() != 0){
                QProgressBar* progressSlider = new QProgressBar(this);
                progressSlider->setMaximum(e->getDuration());
                progressSlider->setValue(e->getProgress());
                setProgressBarStyle(progressSlider);
                e->setProgressBar(progressSlider);

                // Set the horizontal stretch for the progress bar
                progressSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

                innerlayout->addWidget(progressSlider, 1); // Allocate 50% of space
            }
            else e->setProgressBar(nullptr);

            layout->addWidget(innerpage);
        }


        QScrollArea* SA = new QScrollArea;
        SA->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        SA->setWidgetResizable(true); // Allow the widget inside the scroll area to resize
        SA->setWidget(page); // Set the page widget inside the scroll area
        s->SetButtonGroup(btngroup);
        ui->stackedWidget->addWidget(SA);
    }


    /*
    for (Section* s : Sections){
        QButtonGroup* btngroup = new QButtonGroup(this);
        ui->Sections->addItem(s->getSectionName());
        QWidget* page = new QWidget(this);
        page->setMaximumSize(QSize(1000,1000));
        page->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        QFormLayout* layout = new QFormLayout(page);


        layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        for (Episode* e : s->getEpisodes()){
            //layout->addWidget(e);
            QWidget* innerpage = new QWidget;
            QHBoxLayout* innerlayout = new QHBoxLayout(innerpage);
            QPushButton* btn = new QPushButton(this);
            btn->setText(e->getEpisodeName());
            btngroup->addButton(btn);
            innerlayout->addWidget(btn);
            if (e->getProgress() != -1 && e->getProgress() != 0){
                QProgressBar* progressSlider = new QProgressBar(this);
                progressSlider->setMaximum(e->getDuration());
                progressSlider->setValue(e->getProgress());
                innerlayout->addWidget(progressSlider);
                layout->addRow(btn,progressSlider);
            }else {
                layout->addRow(btn);
            }
        }

        QScrollArea* SA = new QScrollArea;
        SA->setStyleSheet("border: 1px solid red");
        SA->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        SA->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        SA->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        SA->setWidget(page);
        s->SetButtonGroup(btngroup);
        ui->stackedWidget->addWidget(SA);
    }
    */

    /*
    for (Section* s : Sections){
        QButtonGroup* btngroup = new QButtonGroup(this);
        ui->Sections->addItem(s->getSectionName());
        QWidget* page = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout(page);


        layout->setAlignment(Qt::AlignTop);
        for (Episode* e : s->getEpisodes()){
            //layout->addWidget(e);
            QWidget* innerpage = new QWidget;
            QHBoxLayout* innerlayout = new QHBoxLayout(innerpage);
            QPushButton* btn = new QPushButton(this);
            btn->setText(e->getEpisodeName());
            btngroup->addButton(btn);
            innerlayout->addWidget(btn);
            if (e->getProgress() != -1 && e->getProgress() != 0){
                QProgressBar* progressSlider = new QProgressBar(this);
                progressSlider->setMaximum(e->getDuration());
                progressSlider->setValue(e->getProgress());
                innerlayout->addWidget(progressSlider);
            }

            layout->addWidget(innerpage);
        }

        QScrollArea* SA = new QScrollArea;
        SA->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        SA->setWidget(page);
        s->SetButtonGroup(btngroup);
        ui->stackedWidget->addWidget(SA);
    }
*/
}

void SeriesOverview::on_Sections_currentIndexChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void SeriesOverview::setProgressBarStyle(QProgressBar *pb)
{
    pb->setStyleSheet(" QProgressBar { border: 2px solid grey; border-radius: 0px; text-align: center; } QProgressBar::chunk {background-color: #3add36; width: 1px;}");
}

void SeriesOverview::UpdateSeries(Series * series)
{

}

void SeriesOverview::UpdateSection(Section * section)
{

}

void SeriesOverview::UpdateEpisode(Episode * episode)
{
    m_MainWindow->AddToPending(episode);

    QWidget* testwidget = ui->stackedWidget->widget(ui->stackedWidget->currentIndex());
    QList<QHBoxLayout*> mylist = testwidget->findChildren<QHBoxLayout*>();

    for (QHBoxLayout* hbl : mylist){
        QPushButton* btn = dynamic_cast<QPushButton*>(hbl->itemAt(0)->widget());
        if (btn == nullptr) return;
        if (btn->text() == episode->getEpisodeName()){
            if (hbl->count() > 1){
                QProgressBar* pb = dynamic_cast<QProgressBar*>(hbl->itemAt(1)->widget());
                pb->setMaximum(episode->getDuration());
                pb->setValue(episode->getProgress());
                if (pb->value() <= 1){
                    hbl->removeWidget(pb);
                    delete pb;
                }
            }
            else{
                QProgressBar* progressSlider = new QProgressBar(this);
                progressSlider->setMaximum(episode->getDuration());
                progressSlider->setValue(episode->getProgress());
                setProgressBarStyle(progressSlider);
                episode->setProgressBar(progressSlider);

                // Set the horizontal stretch for the progress bar
                progressSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

                hbl->addWidget(progressSlider, 1); // Allocate 50% of space
            }
        }
    }
}

