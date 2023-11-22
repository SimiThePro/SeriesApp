#ifndef SERIESOVERVIEW_H
#define SERIESOVERVIEW_H

#include <QWidget>

namespace Ui {
class SeriesOverview;
}

class SeriesOverview : public QWidget
{
    Q_OBJECT

public:
    explicit SeriesOverview(QWidget *parent = nullptr);
    ~SeriesOverview();

    SeriesOverview(class Series* series, QWidget* parent = nullptr);

    void SetSeries(class Series* series);
private slots:
    void on_Sections_currentIndexChanged(int index);

private:
    Ui::SeriesOverview *ui;

    Series* m_DisplaySeries;
};

#endif // SERIESOVERVIEW_H
