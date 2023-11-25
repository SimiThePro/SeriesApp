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
    SeriesIconPath(SeriesIconPath)
{
    ui->setupUi(this);

    m_MainWindow = qobject_cast<MainWindow*>(parent);


    if (SeriesName.isEmpty()){
        //TODO
    }
    else this->SeriesName = SeriesName;

    setSeriesPath(SeriesPath);

    ui->label->setText(SeriesName);
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
    return SeriesName;
}

void Series::setSeriesName(const QString &newSeriesName)
{
    SeriesName = newSeriesName;
    ui->label->setText(newSeriesName);
}

void Series::SaveToFile()
{

    QFile file(static_cast<QString>(PROJECT_PATH) + "Data/Data.json");
    if (!file.open(QIODevice::ReadWrite)){
        qInfo() << "NO Open";
    }

    QByteArray jsonData = file.readAll();


    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (doc.isNull() || !doc.isArray()) {
        qDebug() << "Failed to parse JSON:" << parseError.errorString();
    }


    QJsonArray dataArray = doc.array();

    QJsonObject SeriesEntry;
    SeriesEntry["SeriesName"] = SeriesName;
    SeriesEntry["SeriesPath"] = SeriesPath;
    SeriesEntry["IconPath"] = SeriesIconPath;

    QJsonObject sectionObject;
    QJsonArray sectionsArray;

    QJsonObject episodeObject;
    QJsonArray episodesArray;

    for (Section* s : m_Sections){
        sectionObject["SectionName"] = s->getSectionName();
        for (Episode* e : s->getEpisodes()){
            episodeObject["FilePath"] = e->getFilePath();
            episodeObject["Progress"] = e->getProgress();
            episodesArray.append(episodeObject);
        }
        sectionObject["Episodes"] = episodesArray;
        sectionsArray.append(sectionObject);
    }

    SeriesEntry["Sections"] = sectionsArray;
    dataArray.append(SeriesEntry);

    doc = QJsonDocument(dataArray);

    file.write((doc.toJson()));
    file.close();

}

QString Series::getSeriesPath() const
{
    return SeriesPath;
}

void Series::setSeriesPath(const QString &newSeriesPath)
{
    SeriesPath = newSeriesPath;

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
    return SeriesIconPath;
}

void Series::setSeriesIconPath(const QString &newSeriesIconPath)
{
    SeriesIconPath = newSeriesIconPath;
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

void Section::SetButtonGroup(QButtonGroup *ButtonGroup)
{
    m_ButtonGroup = ButtonGroup;
    connect(m_ButtonGroup,SIGNAL(buttonPressed(QAbstractButton*)),this,SLOT(buttonPressed(QAbstractButton*)));
}

void Section::buttonPressed(QAbstractButton *button)
{
    int index =  (m_ButtonGroup->id(button) * -1) -2;

    m_MainWindow->SeriesSelected(m_FileList[index].absoluteFilePath());

}

Episode::Episode(const QString &FilePath, MainWindow *mainWindow)
{
    m_FileInfo = QFileInfo(FilePath);
    m_MainWindow = mainWindow;
    m_duration = -1; //Set -1 so we know that it is not initialized
    m_progress = -1;
}
