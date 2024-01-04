#ifndef SERIESOVERVIEW_H
#define SERIESOVERVIEW_H

#include <QWidget>
#include <Series.h>

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

    template<typename T>
    void Update(T *ComponentToUpdate)
    {

        if (std::is_same<T,Series>::value){ //Update Series
            Series* series = qobject_cast<Series*>(ComponentToUpdate);
            if (series == nullptr) return;
            UpdateSeries(series);
        }else if (std::is_same<T,Section>::value){ //Update Section
            Section* section = qobject_cast<Section*>(ComponentToUpdate);
            if (section == nullptr) return;
            UpdateSection(section);
        }else if (std::is_same<T,Episode>::value){ //Update Episode
            Episode* episode = qobject_cast<Episode*>(ComponentToUpdate);
            if (episode == nullptr) return;
            UpdateEpisode(episode);
        }

    }


private slots:
    void on_Sections_currentIndexChanged(int index);

private:
    Ui::SeriesOverview *ui;

    MainWindow* m_MainWindow;
    Series* m_DisplaySeries;

    void setProgressBarStyle(class QProgressBar* pb);

    void UpdateSeries(class Series*);
    void UpdateSection(class Section*);
    void UpdateEpisode(class Episode*);
};

#endif // SERIESOVERVIEW_H
