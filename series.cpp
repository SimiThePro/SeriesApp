#include "series.h"
#include "SeriesOverview.h"
#include "mainwindow.h"
#include "overview.h"
#include "ui_series.h"
#include "ui_episode.h"

#include <AddSeries.h>
#include <CustomPushButton.h>
#include <Library.h>
#include <QCollator>
#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QStackedLayout>
#include <QString>
#include <qbuttongroup.h>
#include "episode.h"

Series::Series(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Series)
{
    ui->setupUi(this);

    m_MainWindow = qobject_cast<MainWindow*>(parent);

    CreateMenu();
}

Series::Series(const QString &SeriesName, const QString &SeriesPath, const QString &IconPath, QList<Section *> sections, QWidget *parent)
    :
    QWidget(parent),
    ui(new Ui::Series)
{
    ui->setupUi(this);

    m_MainWindow = qobject_cast<MainWindow*>(parent);

    m_SeriesName = SeriesName;
    ui->label->setText(SeriesName);

    m_SeriesPath = SeriesPath;

    m_SeriesIconPath = IconPath;
    setButtonImage(IconPath);

    m_PathLastEpisodeWatched = "";
    m_LastWatched = DateAndTime::Null();

    m_Sections = sections;
    for (auto& s : m_Sections){
        s->setSeries(this);
        for (auto& e : s->getEpisodes()){
            e->setSection(s);
        }
    }

    CreateMenu();
}

Series::~Series()
{
    delete ui;
}

void Series::on_pushButton_pressed()
{

    m_MainWindow->SeriesPressed(this);
}

void Series::on_editSeries()
{
    AddSeries* EditSeries = new AddSeries(m_MainWindow,this);
    EditSeries->exec();
}

void Series::CreateMenu()
{
    QMenu* menu = new QMenu(this);
    EditSeries = new QAction("Edit Series",this);
    connect(EditSeries,SIGNAL(triggered(bool)),this,SLOT(on_editSeries()));

    ui->toolButton->addAction(EditSeries);
    ui->toolButton->setPopupMode(QToolButton::InstantPopup);
}




QString Series::getSeriesName() const
{
    return m_SeriesName;
}

void Series::setSeriesName(const QString &newSeriesName)
{
    m_SeriesName = newSeriesName;
    ui->label->setText(newSeriesName);
}

void Series::UpdateInFile()
{
    QJsonObject obj;
    QString path;
    QJsonValue newValue;

    QJsonArray jsonArray = m_MainWindow->getSeriesJsonArray();

    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject seriesObject = jsonArray[i].toObject();
        if (seriesObject["SeriesPath"].toString() == m_SeriesPath){
            seriesObject["LastWatched"] = m_LastWatched->toString();
            seriesObject["LastEpisodePath"] = m_PathLastEpisodeWatched;

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
            return;
        }
    }
}

void Series::SaveToFile()
{

    QJsonObject SeriesEntry;
    SeriesEntry["SeriesName"] = m_SeriesName;
    SeriesEntry["SeriesPath"] = m_SeriesPath;
    SeriesEntry["IconPath"] = m_SeriesIconPath;
    SeriesEntry["LastWatched"] = m_LastWatched->toString();
    SeriesEntry["LastEpisodePath"] = m_PathLastEpisodeWatched;

    QJsonObject sectionObject;
    QJsonArray sectionsArray;
    QJsonArray episodesArray;
    QJsonObject episodeObject;


    for (Section* s : m_Sections){
        sectionObject["SectionName"] = s->getSectionName();
        for (Episode* e : s->getEpisodes()){
            episodeObject["FilePath"] = e->getFilePath();
            episodeObject["Progress"] = e->getProgress();
            episodeObject["Duration"] = e->getDuration();
            episodesArray.append(episodeObject);
            e->setJsonObject(episodeObject);
        }
        sectionObject["Episodes"] = episodesArray;
        sectionsArray.append(sectionObject);
        while (!episodesArray.isEmpty()){
            episodesArray.removeAt(0);
        }

    }
    SeriesEntry["Sections"] = sectionsArray;

    m_MainWindow->AddToJSON(SeriesEntry);

}

Episode *Series::getEpisodeFromFilePath(const QString &Path)
{
    for (Section* section : m_Sections){
        QList<Episode*> Episodes = section->getEpisodes();
        for (Episode* episode : Episodes){
            if (episode->getFilePath() == Path){
                return episode;
            }
        }
    }
    return nullptr;
}

QString Series::getSeriesPath() const
{
    return m_SeriesPath;
}

void Series::setSeriesPath(const QString &newSeriesPath)
{
    m_SeriesPath = newSeriesPath;

    m_Sections.clear();

    QDir dir(newSeriesPath);

    QList<QFileInfo> fileList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);


    // Find the first index of a digit in the string, so we have a reference for sorting
    for (int i = 0; i< fileList.size(); i++){
        bool finished = false;
        for (int j = 0; j < fileList[i].completeBaseName().length();j++){
            if (fileList[i].completeBaseName()[j].isDigit()){
                m_SectionNumberIndex = j;
                finished = true;
                break;
            }
        }
        if (finished){
            break;
        }
    }


    std::sort(fileList.begin(),fileList.end(),
              [&](const QFileInfo &FileInfo1, const QFileInfo &FileInfo2){
                  QString s1 = FileInfo1.completeBaseName() + FileInfo1.completeSuffix();
                  QString s2 = FileInfo2.completeBaseName() + FileInfo2.completeSuffix();

                  int i1 = -1, i2 = -1;

                  for (int i = m_SectionNumberIndex; i < s1.length();i++){
                      if (!s1[i].isDigit()){
                          i1 = s1.sliced(m_SectionNumberIndex,i-m_SectionNumberIndex).toInt();
                          break;
                      }
                  }

                  for (int i = m_SectionNumberIndex; i < s2.length();i++){
                      if (!s2[i].isDigit()){
                          i2 = s2.sliced(m_SectionNumberIndex,i-m_SectionNumberIndex).toInt();
                          break;
                      }
                  }

                  return i1 < i2;
              });


    for (auto& info : fileList){
        m_Sections.append(new Section(QDir(info.filePath()),m_MainWindow));
    }


}

QString Series::getSeriesIconPath() const
{
    return m_SeriesIconPath;
}

void Series::setSeriesIconPath(const QString &newSeriesIconPath)
{
    m_SeriesIconPath = newSeriesIconPath;
    setButtonImage(newSeriesIconPath);
}



void Series::setButtonImage(const QString &filename)
{
    QString filenamecopy;
    QString stylesheet = ui->pushButton->styleSheet();



    if (stylesheet.contains("border-image: url(")){

        qInfo() << stylesheet.indexOf("border-image: url(");
        qInfo() << stylesheet.indexOf(");",stylesheet.indexOf("border-image: url("));

        QString oldPath =
            stylesheet.sliced(stylesheet.indexOf("border-image: url(") + QString("border-image: url(").size(),
                              stylesheet.indexOf(");",stylesheet.indexOf("border-image: url(")) - stylesheet.indexOf("border-image: url(") - QString("border-image: url(").size());

        if (QUrl::fromLocalFile(filename).isValid()){
            stylesheet.replace(oldPath,filename);
            ui->pushButton->setStyleSheet(stylesheet);
        }
    }else qInfo() << "Error";
}

//**Section***************

Section::Section(QDir dir, MainWindow* mainWindow)
{
    createActions();
    m_SectionName = dir.dirName();
    m_MainWindow = mainWindow;
    QList<QFileInfo> fileList = dir.entryInfoList(QStringList() << "*.mp4",QDir::NoFilter,QDir::Name);

    int EpiodesNumberIndex = 0;

    // Find the first index of a digit in the string, so we have a reference for sorting
    for (int i = 0; i< fileList.size(); i++){
        bool finished = false;
        for (int j = 0; j < fileList[i].completeBaseName().length();j++){
            if (fileList[i].completeBaseName()[j].isDigit()){
                EpiodesNumberIndex = j;
                finished = true;
                break;
            }
        }
        if (finished){
            break;
        }
    }


    std::sort(fileList.begin(),fileList.end(),
              [&](const QFileInfo &FileInfo1, const QFileInfo &FileInfo2){
                  QString s1 = FileInfo1.completeBaseName() + FileInfo1.completeSuffix();
                  QString s2 = FileInfo2.completeBaseName() + FileInfo2.completeSuffix();

                  int i1, i2;

                  for (int i = EpiodesNumberIndex; i < s1.length();i++){
                      if (!s1[i].isDigit()){
                          i1 = s1.sliced(EpiodesNumberIndex,i-EpiodesNumberIndex).toInt();
                          break;
                      }
                  }

                  for (int i = EpiodesNumberIndex; i < s2.length();i++){
                      if (!s2[i].isDigit()){
                          i2 = s2.sliced(EpiodesNumberIndex,i-EpiodesNumberIndex).toInt();
                          break;
                      }
                  }

                  return i1 < i2;
              });

    for (auto& a : fileList){
        m_VideoFiles.append(a.filePath());
        Episode* newEpisode = new Episode(a.filePath(),m_MainWindow);
        newEpisode->setSection(this);
        m_Episodes.append(newEpisode);
    }

    m_FileList = fileList;

}

Section::Section(const QString &SectionName, QList<Episode *> Episodes, MainWindow *mainWindow)
{
    createActions();
    m_SectionName = SectionName;
    m_Episodes = Episodes;
    m_MainWindow = mainWindow;
}

void Section::SetButtonGroup(QButtonGroup *ButtonGroup)
{
    m_ButtonGroup = ButtonGroup;
    connect(m_ButtonGroup,SIGNAL(buttonPressed(QAbstractButton*)),this,SLOT(buttonPressed(QAbstractButton*)));

}

void Section::on_FinishedWatching()
{
    qInfo() << "Finished Watching";
    Episode* ep = m_Episodes[m_pressedEpisodeIndex];
    if (ep->getDuration() < 0){
        ep->setDuration(2);
        ep->setProgess(2);
    }else {
        ep->setProgess(ep->getDuration());
    }

    m_MainWindow->getSeriesOverviewWidget()->Update(ep);

}

void Section::on_ResetProgress()
{
    qInfo() << "Reset Progress";
    Episode* ep = m_Episodes[m_pressedEpisodeIndex];
    ep->setProgess(0);
    m_MainWindow->getSeriesOverviewWidget()->Update(ep);
}


void Section::createActions()
{
    FinishedWatching = new QAction("Finished Wacthing",this);
    FinishedWatching->setStatusTip(tr("Sets the status of the episode as finished"));
    connect(FinishedWatching,&QAction::triggered,this,&Section::on_FinishedWatching);
    ResetProgress = new QAction("Reset Progress",this);
    connect(ResetProgress,&QAction::triggered,this,&Section::on_ResetProgress);
}


void Section::buttonPressed(QAbstractButton *button)
{
    int index =  (m_ButtonGroup->id(button) * -1) -2;
    m_pressedEpisodeIndex = index;

    CustomPushButton* cpb = qobject_cast<CustomPushButton*>(button);

    if (cpb == nullptr) return;

    if (cpb->getPressType() == RightClick){
        //RightClick
        QMenu *menu = new QMenu(m_MainWindow);
        menu->addAction(FinishedWatching);
        menu->addAction(ResetProgress);
        menu->popup(QCursor::pos());
    }else {
        //LeftClick
        m_MainWindow->EpisodeSelected(m_Episodes[index]);
    }

}


