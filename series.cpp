#include "series.h"
#include "mainwindow.h"
#include "ui_series.h"


#include <Library.h>
#include <QCollator>
#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStackedLayout>
#include <QString>
#include <qbuttongroup.h>

Series::Series(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Series)
{
    ui->setupUi(this);

    m_MainWindow = qobject_cast<MainWindow*>(parent);


}

Series::Series(QString SeriesPath, QString SeriesIconPath, QString SeriesName, QWidget *parent)
    :QWidget(parent),
    ui(new Ui::Series),
    m_SeriesIconPath(SeriesIconPath)
{
    ui->setupUi(this);

    m_MainWindow = qobject_cast<MainWindow*>(parent);


    if (SeriesName.isEmpty()){
        //TODO
    }
    else this->m_SeriesName = SeriesName;

    setSeriesPath(SeriesPath);

    ui->label->setText(SeriesName);
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
    m_Sections = sections;
    for (auto& s : m_Sections){
        s->setSeries(this);
        for (auto& e : s->getEpisodes()){
            e->setSection(s);
        }
    }

}

Series::~Series()
{
    delete ui;
}




void Series::on_pushButton_pressed()
{
    m_MainWindow->SeriesPressed(this);
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

void Series::SaveToFile()
{

    QJsonObject SeriesEntry;
    SeriesEntry["SeriesName"] = m_SeriesName;
    SeriesEntry["SeriesPath"] = m_SeriesPath;
    SeriesEntry["IconPath"] = m_SeriesIconPath;

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
        m_Episodes.append(new Episode(a.filePath(),m_MainWindow));
    }

    m_FileList = fileList;

}

Section::Section(const QString &SectionName, QList<Episode *> Episodes, MainWindow *mainWindow)
{
    m_SectionName = SectionName;
    m_Episodes = Episodes;
    m_MainWindow = mainWindow;
}

void Section::SetButtonGroup(QButtonGroup *ButtonGroup)
{
    m_ButtonGroup = ButtonGroup;
    connect(m_ButtonGroup,SIGNAL(buttonPressed(QAbstractButton*)),this,SLOT(buttonPressed(QAbstractButton*)));
}


void Section::buttonPressed(QAbstractButton *button)
{
    int index =  (m_ButtonGroup->id(button) * -1) -2;

    m_MainWindow->EpisodeSelected(m_Episodes[index]);
}


Episode::Episode(const QString &FilePath, MainWindow *mainWindow)
{
    m_FileInfo = QFileInfo(FilePath);
    m_EpisodeName = m_FileInfo.completeBaseName();
    m_MainWindow = mainWindow;
    m_duration = -1; //Set -1 so we know that it is not initialized
    m_progress = -1;
}

Episode::Episode(const QString &FilePath, int progress, int duration, MainWindow *mainWindow)
{

    m_FileInfo = QFileInfo(FilePath);
    m_EpisodeName = m_FileInfo.completeBaseName();
    m_progress = progress;
    m_duration = duration;
    m_MainWindow = mainWindow;
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
