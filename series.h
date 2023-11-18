#ifndef SERIES_H
#define SERIES_H

#include "qurl.h"
#include <QWidget>

namespace Ui {
class Series;
}

class Series : public QWidget
{
    Q_OBJECT

public:
    explicit Series(QWidget *parent = nullptr);

    ~Series();



    QPixmap getSeriesIcon() const;
    void setSeriesIcon(QPixmap newSeriesIcon);

    QUrl getSeriesPath() const;
    void setSeriesPath(QUrl newSeriesPath);

    QString getSeriesName() const;
    void setSeriesName(const QString &newSeriesName);

private:
    Ui::Series *ui;

    QPixmap SeriesIcon;
    QUrl SeriesPath;
    QString SeriesName;


};

#endif // SERIES_H
