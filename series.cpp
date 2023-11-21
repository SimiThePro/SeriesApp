#include "series.h"
#include "ui_series.h"

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

}

Series::Series(QString SeriesPath, QString SeriesIconPath, QString SeriesName, QWidget *parent)
    :QWidget(parent),
    ui(new Ui::Series),
    SeriesIconPath(SeriesIconPath)
{
    ui->setupUi(this);



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
    qInfo() << "Pressed";
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

    // TODO     Fix Sorting (it sorts by first digit not by whole number)


    QList<QFileInfo> fileList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    /*
    QCollator collator;
    std::sort(
        fileList.begin(),
        fileList.end(),
        [&](const QFileInfo &file1, const QFileInfo &file2){
            return collator.compare(file1.filePath(),file2.filePath()) < 0;
        });
    */


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
    QList<QFileInfo> fileList = dir.entryInfoList(QStringList() << "*.mp4",QDir::NoFilter,QDir::Name);


    for (auto& a : fileList){
        m_VideoFiles.append(a.filePath());
    }

}
