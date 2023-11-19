#include "series.h"
#include "ui_series.h"

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
    SeriesIconPath(SeriesIconPath),
    SeriesPath(SeriesPath)
{
    ui->setupUi(this);

    if (SeriesName.isEmpty()){
        //TODO
    }
    else this->SeriesName = SeriesName;

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

QString Series::getSeriesPath() const
{
    return SeriesPath;
}

void Series::setSeriesPath(const QString &newSeriesPath)
{
    SeriesPath = newSeriesPath;
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



    if (stylesheet.contains("background-image: url(")){

        qInfo() << stylesheet.indexOf("background-image: url(");
        qInfo() << stylesheet.indexOf(");",stylesheet.indexOf("background-image: url("));

        QString oldPath =
            stylesheet.sliced(stylesheet.indexOf("background-image: url(") + QString("background-image: url(").size(),
                              stylesheet.indexOf(");",stylesheet.indexOf("background-image: url(")) - stylesheet.indexOf("background-image: url(") - QString("background-image: url(").size());

        if (QUrl::fromLocalFile(filename).isValid()){
            stylesheet.replace(oldPath,filename);
            ui->pushButton->setStyleSheet(stylesheet);
        }
    }else qInfo() << "Error";
}


