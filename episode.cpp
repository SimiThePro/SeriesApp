#include "episode.h"
#include "mainwindow.h"
#include "series.h"
#include "ui_episode.h"

#include <QJsonDocument>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>

Episode::Episode(const QString &FilePath, MainWindow *mainWindow)
{
    ui = new Ui::Episode();
    m_FileInfo = QFileInfo(FilePath);
    m_EpisodeName = m_FileInfo.completeBaseName();
    m_MainWindow = mainWindow;
    m_duration = -1; //Set -1 so we know that it is not initialized
    m_progress = -1;
    //InitializeUi();
}

Episode::Episode(const QString &FilePath, int progress, int duration, MainWindow *mainWindow)
{
    ui = new Ui::Episode();
    m_FileInfo = QFileInfo(FilePath);
    m_EpisodeName = m_FileInfo.completeBaseName();
    m_progress = progress;
    m_duration = duration;
    m_MainWindow = mainWindow;
    //InitializeUi();
}

Episode::~Episode(){
    delete ui;
}

void Episode::setProgess(int newProgress)
{
    m_progress = newProgress;

}

void Episode::UpdateValue()
{
    QJsonObject obj;
    QString path;
    QJsonValue newValue;

    QJsonArray jsonArray = m_MainWindow->getSeriesJsonArray();

    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject seriesObject = jsonArray[i].toObject();
        QJsonArray sectionsArray = seriesObject["Sections"].toArray();

        for (int j = 0; j < sectionsArray.size(); ++j) {
            QJsonObject sectionObject = sectionsArray[j].toObject();

            if (sectionObject["SectionName"].toString() == m_Section->getSectionName()) {
                QJsonArray episodesArray = sectionObject["Episodes"].toArray();

                for (int k = 0; k < episodesArray.size(); ++k) {
                    QJsonObject episodeObject = episodesArray[k].toObject();

                    if (episodeObject["FilePath"].toString() == m_FileInfo.filePath()) {
                        episodeObject["Duration"] = m_duration;
                        episodeObject["Progress"] = (m_progress == 0) ? -1 : m_progress;
                        episodesArray[k] = episodeObject;
                        sectionObject["Episodes"] = episodesArray;
                        sectionsArray[j] = sectionObject;
                        seriesObject["Sections"] = sectionsArray;
                        jsonArray[i] = seriesObject;

                        QFile writeFile(static_cast<QString>(PROJECT_PATH) + "Data/Data.json");
                        if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                            qDebug() << "Failed to open file for writing:" << writeFile.errorString();
                            return;
                        }

                        m_MainWindow->setSeriesJsonArray(jsonArray);

                        QJsonDocument updatedDoc(jsonArray);
                        writeFile.write(updatedDoc.toJson());
                        writeFile.close();
                        qDebug() << "Duration updated for file" << m_FileInfo.filePath();
                        return;
                    }
                }
            }
        }
    }


    /*

    for (auto a: m_MainWindow->getSeriesJsonArray()){
        QJsonObject seriesObject = a.toObject();
        if (seriesObject.contains("Sections")){
            QJsonArray sectionsArray = seriesObject["Sections"].toArray();
            if (sectionsArray.contains(m_Section->getSectionName())){
                int index = 0;
                for (auto s : sectionsArray){

                    if (s.toString() == m_Section->getSectionName()){

                    }
                    index++;
                }
            }
        }
    }




    const int indexOfDot = path.indexOf('.');
    const QString propertyName = path.left(indexOfDot);
    const QString subPath = indexOfDot>0 ? path.mid(indexOfDot+1) : QString();

    QJsonValue subValue = obj[propertyName];

    if(subPath.isEmpty()) {
        subValue = newValue;
    }
    else {
        QJsonObject obj = subValue.toObject();
        //modifyJsonValue(obj,subPath,newValue);
        subValue = obj;
    }

    obj[propertyName] = subValue;

*/
}

void Episode::InitializeUi()
{
    QPushButton *Button = new QPushButton;
    QLabel *Name = new QLabel(m_EpisodeName);

    QStackedLayout *stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(Button);
    stackedLayout->addWidget(Name);
    stackedLayout->setStackingMode(QStackedLayout::StackAll);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(stackedLayout);
    setLayout(mainLayout);

}
