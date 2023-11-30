
#include "mainwindow.h"
#include "episode.h"
#include "ui_mainwindow.h"
#include <Library.h>
#include <QScrollArea>
#include <overview.h>
#include <QScrollBar>
#include <AddSeries.h>
#include <Series.h>
#include <SeriesOverview.h>
#include <VideoPlayer.h>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OverviewWidget = new Overview(this);
    LibraryWidget = new Library(this);
    m_SeriesOverviewWidget = new SeriesOverview(this);
    m_VideoPlayerWidget = new VideoPlayer(this);

    m_SeriesList = QList<Series*>{};
    m_pendingChangedEpisodes = QList<Episode*>{};


    ui->stackedWidget->addWidget(OverviewWidget);
    ui->stackedWidget->addWidget(LibraryWidget);
    ui->stackedWidget->addWidget(m_SeriesOverviewWidget);
    ui->stackedWidget->addWidget(m_VideoPlayerWidget);
    ui->stackedWidget->setCurrentWidget(OverviewWidget);


    //Load Series from File and add them to library
    LoadFromFileAndAddSeries();

}

MainWindow::~MainWindow()
{
    for (auto e : m_pendingChangedEpisodes){
        e->UpdateValue();
    }
    delete ui;
}

void MainWindow::AddSeries(Series *newSeries)
{
    if (!newSeries) return;

    m_SeriesList.append(newSeries);

    LibraryWidget->AddSeriesToLayout(newSeries);
    newSeries->SaveToFile();

}

void MainWindow::SeriesPressed(Series *pressedSeries)
{
    pressedSeries->MainWindowParent(this);
    m_SeriesOverviewWidget->SetSeries(pressedSeries);
    ui->stackedWidget->setCurrentWidget(m_SeriesOverviewWidget);
}

void MainWindow::EpisodeSelected(Episode* episode)
{
    m_pendingChangedEpisodes.append(episode);
    episode->setDuration(50);
    m_VideoPlayerWidget->SetEpisode(episode);
    ui->stackedWidget->setCurrentWidget(m_VideoPlayerWidget);
}

void MainWindow::LoadFromFileAndAddSeries()
{
    QFile file(static_cast<QString>(PROJECT_PATH) + "Data/Data.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << file.errorString();
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (doc.isNull() || !doc.isArray()) {
        qDebug() << "Failed when creating:" << parseError.errorString();
        if (parseError.error != QJsonParseError::IllegalValue &&
            parseError.error != QJsonParseError::GarbageAtEnd){
            return;
        }
    }
    jsonData.clear();

    QJsonArray arr = doc.array();



    for (const auto& series : arr){
        QJsonObject seriesObject = series.toObject();

        QList<Section*> sections = {};
        QString SeriesName = seriesObject["SeriesName"].toString();
        QString IconPath = seriesObject["IconPath"].toString();
        QString SeriesPath = seriesObject["SeriesPath"].toString();
        // Set Course Path

        if (seriesObject.contains("Sections") && seriesObject["Sections"].isArray()){
            QJsonArray sectionArray = seriesObject["Sections"].toArray();

            //Iterate through sections
            for (const auto& section : sectionArray){
                QJsonObject sectionObject = section.toObject();
                QList<Episode*> episodes = {};
                if (sectionObject.contains("Episodes") && sectionObject["Episodes"].isArray()){
                    QJsonArray episodesArray = sectionObject["Episodes"].toArray();

                    // Iterate through the episodes
                    for (const auto &episode : episodesArray) {
                        QJsonObject episodeObject = episode.toObject();

                        QString filePath = episodeObject["FilePath"].toString();
                        int progress = episodeObject["Progress"].toInt();
                        int duration = episodeObject["Duration"].toInt();
                        //Add episode to episode list for the section
                        episodes.append(new Episode(filePath,progress,duration,this));
                    }
                }

                QString sectionName = sectionObject["SectionName"].toString();
                sections.append(new Section(sectionName,episodes,this));
            }
        }

        SeriesJsonArray.append(seriesObject);
        Series* loadedSeries = new Series(SeriesName,SeriesPath,IconPath,sections,this);
        m_SeriesList.append(loadedSeries);
        LibraryWidget->AddSeriesToLayout(loadedSeries);
    }
}

void MainWindow::AddToJSON(QJsonObject newObject)
{
    SeriesJsonArray.append(newObject);
    UpdateJSON();
}

void MainWindow::UpdateJSON()
{
    QFile file(static_cast<QString>(PROJECT_PATH) + "Data/Data.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << file.errorString();
    }

    QJsonDocument doc(SeriesJsonArray);
    file.write(doc.toJson());
    file.close();
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
    class AddSeries* AddSeriesWidget = new class AddSeries(this);

    AddSeriesWidget->exec();
}


void MainWindow::on_VideoPlayerButton_pressed()
{
    ui->stackedWidget->setCurrentWidget(m_VideoPlayerWidget);
}

