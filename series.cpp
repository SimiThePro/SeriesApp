#include "series.h"
#include "mainwindow.h"
#include "ui_series.h"

#include <Library.h>
#include <QCollator>
#include <QDir>
#include <QDirIterator>
#include <QStackedLayout>
#include <QString>

Series::Series(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Series)
{
    ui->setupUi(this);

    m_MainWindow = qobject_cast<MainWindow*>(parent);

    if (m_MainWindow == nullptr) qInfo() << "Error";
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

QString Series::PrintFiles()
{
    QString rS = "";

    for (auto& section : m_Sections){

    }
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
        m_Sections.append(new Section(QDir(info.filePath())));
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

Section::Section(QDir dir)
{
    m_SectionName = dir.dirName();

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
    }

}
