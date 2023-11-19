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
    Series(QString SeriesPath, QString SeriesIconPath, QString SeriesName = "", QWidget *parent = nullptr);
    ~Series();

    void setButtonImage(const QString& filename);
    QString getSeriesIconPath() const;
    void setSeriesIconPath(const QString &newSeriesIconPath);

    QString getSeriesPath() const;
    void setSeriesPath(const QString &newSeriesPath);

    QString getSeriesName() const;
    void setSeriesName(const QString &newSeriesName);

protected:

private slots:
    void on_pushButton_pressed();

private:
    Ui::Series *ui;

    QString SeriesIconPath;
    QString SeriesPath;
    QString SeriesName;


    void SetButtonIcon(const QString& filename);
};

#endif // SERIES_H
