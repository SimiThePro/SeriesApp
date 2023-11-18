#include "series.h"
#include "ui_Series.h"

Series::Series(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Series)
{
    ui->setupUi(this);


    ui->pushButton->setIcon(QIcon(SeriesIcon));
    ui->pushButton->setIconSize(ui->pushButton->maximumSize());
    ui->label->setText(SeriesName);
}

Series::~Series()
{
    delete ui;
}

QPixmap Series::getSeriesIcon() const
{
    return SeriesIcon;
}

void Series::setSeriesIcon(QPixmap newSeriesIcon)
{
    SeriesIcon = newSeriesIcon;
    ui->pushButton->setIcon(QIcon(SeriesIcon));
}

QUrl Series::getSeriesPath() const
{
    return SeriesPath;
}

void Series::setSeriesPath(QUrl newSeriesPath)
{
    SeriesPath = newSeriesPath;
}

QString Series::getSeriesName() const
{
    return SeriesName;
}

void Series::setSeriesName(const QString &newSeriesName)
{
    SeriesName = newSeriesName;
    ui->label->setText(SeriesName);
}
