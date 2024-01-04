
#include "mainwindow.h"
#include "episode.h"
#include "qwindow.h"
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
#include <QScreen>
#include <windows.h>
#include <winuser.h>
#include <QProcess>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OverviewWidget = new Overview(this);
    LibraryWidget = new Library(this);
    m_SeriesOverviewWidget = new SeriesOverview(this);

    m_SeriesList = QList<Series*>{};
    m_pendingChangedEpisodes = QList<Episode*>{};

    ui->stackedWidget->addWidget(OverviewWidget);
    ui->stackedWidget->addWidget(LibraryWidget);
    ui->stackedWidget->addWidget(m_SeriesOverviewWidget);
    ui->stackedWidget->setCurrentWidget(OverviewWidget);

    //Load Series from File and add them to library
    LoadFromFileAndAddSeries();

    m_MPVVideoPlayer = new QProcess;


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
    hide();

    AddToPending(episode);
    m_currentEpisode = episode;
    QStringList arguments;
    arguments << (
        (episode->getProgress() > 0 &&
        episode->getProgress() <= episode->getDuration()-5) ?
        "--start=" + QString::fromStdString(std::to_string(episode->getProgress()))
        : "")
        << episode->getFilePath();

    m_MPVVideoPlayer = new QProcess(this);
    m_MPVVideoPlayer->start("C:/Users/simim/Documents/MPV/mpv.com", arguments);
    connect(m_MPVVideoPlayer,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(on_MPVVideoPlayer_closed(int,QProcess::ExitStatus)));

    return;
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

void MainWindow::SimpleUpdateSeries(Series *UpdatedSeries)
{
    QJsonArray jsonArray = SeriesJsonArray;

    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject seriesObject = jsonArray[i].toObject();
        if (seriesObject["SeriesPath"].toString() == UpdatedSeries->getSeriesPath()){
            seriesObject["SeriesName"] = UpdatedSeries->getSeriesName();
            seriesObject["IconPath"]= UpdatedSeries->getSeriesIconPath();

            jsonArray[i] = seriesObject;

            QFile writeFile(static_cast<QString>(PROJECT_PATH) + "Data/Data.json");
            if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                qDebug() << "Failed to open file for writing:" << writeFile.errorString();
                return;
            }

            setSeriesJsonArray(jsonArray);

            QJsonDocument updatedDoc(jsonArray);
            writeFile.write(updatedDoc.toJson());
            writeFile.close();
            return;
        }


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

void MainWindow::AddToPending(Episode *pendingEpisode)
{
    if (m_pendingChangedEpisodes.contains(pendingEpisode)) return;
    m_pendingChangedEpisodes.append(pendingEpisode);
}

void MainWindow::MakeVideoPlayerFullscreen()
{

    return;
    // Show the video widget on a specific screen in full-screen mode
    QList<QScreen*> screens = QApplication::screens();
    if (screens.size() > 1) { // If more than one screen is available
        // Assuming the second screen is the desired screen (index 1)
        QScreen *desiredScreen = screens.at(1);
        m_VideoPlayerWidget->setWindowState(Qt::WindowFullScreen); // Set the widget in full-screen mode
        m_VideoPlayerWidget->setGeometry(desiredScreen->geometry()); // Set the widget geometry to cover the screen


    }

    m_VideoPlayerWidget->show(); // Show the video widget
}

void MainWindow::ReturnToSeriesOverviewWidget()
{
    ui->stackedWidget->setCurrentWidget(m_SeriesOverviewWidget);
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

void MainWindow::on_MPVVideoPlayer_closed(int,QProcess::ExitStatus)
{

    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    QDir* roamingpath = new QDir(paths[0]);
    roamingpath->cdUp();

    QStringList lines;
    QStringList linesToDelete;
    QString path = roamingpath->absolutePath() + "/mpv/mpvHistory.log";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    float duration = 0.f;
    float time = 0.f;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        lines << line;
        if (line.contains(m_currentEpisode->getFilePath())){
            linesToDelete << line;
            duration = line.section('|',1,1).section('=',1,1).toFloat();
            time = line.section('|',2,2).section('=',1,1).toFloat();
        }
    }
    file.close();
    for (auto line : linesToDelete){
        lines.removeAll(line);
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        // Failed to open the file for writing
        return;
    }

    QTextStream out(&file);
    for (const QString& line : lines) {
        out << line << '\n';
    }
    file.close();

    m_currentEpisode->setDuration(duration);
    m_currentEpisode->setProgess(time);

    m_SeriesOverviewWidget->Update(m_currentEpisode);

    showNormal();

    return;
}

