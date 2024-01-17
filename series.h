#ifndef SERIES_H
#define SERIES_H

#include "qurl.h"
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QWidget>

struct DateAndTime{

    DateAndTime():Date(QDate::fromString("01.01.2000","dd.MM.yyyy")),Time(QTime::fromString("00:00:00","hh:mm:ss")){};
    DateAndTime(DateAndTime* dat):Date(dat->Date),Time(dat->Time){}
    DateAndTime(QDate date,QTime time):Date(date),Time(time){}
    QDate Date;
    QTime Time;

    bool operator<(const DateAndTime& other) const {
        // Compare dates first
        if (Date < other.Date)
            return true;
        else if (Date > other.Date)
            return false;

        // If dates are equal, compare times
        return Time < other.Time;
    }

    static DateAndTime* fromString(const QString& string){
        if (string == "") return DateAndTime::Null();

        DateAndTime* retValue = new DateAndTime();
        retValue->Date = QDate::fromString(string.section('|',0,0),"dd.MM.yyyy");
        retValue->Time = QTime::fromString(string.section('|',1,1),"hh:mm:ss");
        return retValue;
    }

    static DateAndTime* Null(){
        DateAndTime* retValue = new DateAndTime();
        retValue->Date = QDate::fromString("01.01.2000","dd.MM.yyyy");
        retValue->Time = QTime::fromString("00:00:00","hh:mm:ss");
        return retValue;
    }

    static DateAndTime* Now(){
        DateAndTime* retValue = new DateAndTime();
        retValue->Date = QDate::currentDate();
        retValue->Time = QTime::currentTime();
        return retValue;
    }

    static bool isValid(DateAndTime* DaT){
        return (DaT->toString() != "01.01.2000|00:00:00");
    }

    QString toString(){
        if (!Date.isValid() || !Time.isValid()){
            return Null()->toString();
        }
        return Date.toString("dd.MM.yyyy") + "|" + Time.toString("hh:mm:ss");
    }


};


class QFileInfo;
namespace Ui {
class Series;
}

class Series : public QWidget
{
    Q_OBJECT

public:
    explicit Series(QWidget *parent = nullptr);
    Series(const QString& SeriesName, const QString& SeriesPath, const QString& IconPath,QList<class Section*> sections, QWidget *parent = nullptr);
    ~Series();

    void setButtonImage(const QString& filename);
    QString getSeriesIconPath() const;
    void setSeriesIconPath(const QString &newSeriesIconPath);

    QString getSeriesPath() const;
    void setSeriesPath(const QString &newSeriesPath);

    QString getSeriesName() const;
    void setSeriesName(const QString &newSeriesName);

    void setSeriesLastWatched(DateAndTime* lastWatched){m_LastWatched = lastWatched;}
    DateAndTime* getSeriesLastWatched() const {return m_LastWatched;}

    void setPathLastEpisodeWatched(const QString& Path){m_PathLastEpisodeWatched = Path;}
    QString getPathLastEpisodeWatched() const {return m_PathLastEpisodeWatched;}

    QString PrintFiles();

    QList<class Section*> getSections() const {return m_Sections;}

    void MainWindowParent(class MainWindow* MW) {this->m_MainWindow = MW;}

    void SetupSeries();

    void UpdateInFile();
    void SaveToFile();

    class Episode* getEpisodeFromFilePath(const QString& Path);
protected:

private slots:
    void on_pushButton_pressed();
    void on_editSeries();


private:
    Ui::Series *ui;

    class MainWindow* m_MainWindow;

    QString m_SeriesIconPath;
    QString m_SeriesPath;
    QString m_SeriesName;
    DateAndTime* m_LastWatched;
    QString m_PathLastEpisodeWatched;

    QList<class Section*> m_Sections;

    int m_SectionNumberIndex = 0;

    void SetButtonIcon(const QString& filename);

    QAction* EditSeries;
    void CreateMenu();

};





//-------------SECTION-------------
class Section : public QObject{

   Q_OBJECT
public:
    Section(class QDir dir, class MainWindow* mainWindow);
    Section(const QString& SectionName,QList<class Episode*> Episodes,MainWindow* mainWindow);
    QStringList getVideoFiles() const {return m_VideoFiles;}

    QString getSectionName() const {return m_SectionName;}

    QList<QFileInfo> getFileList() {return m_FileList;}

    void SetButtonGroup(class QButtonGroup* ButtonGroup);
    QList<class Episode*> getEpisodes() const {return m_Episodes;}
    void setSeries(class Series* series) {m_Series = series;}
    Series* getSeries() const {return m_Series;}
private:

    MainWindow* m_MainWindow;

    class Series* m_Series;

    QString m_SectionName;
    QStringList m_VideoFiles;

    class QButtonGroup* m_ButtonGroup;

    QVector<QFileInfo> m_FileList;
    QList<class Episode*> m_Episodes;

    void createActions();
    QAction* FinishedWatching;
    QAction* ResetProgress;

    int m_pressedEpisodeIndex;

private slots:
    void buttonPressed(class QAbstractButton *button);

    void on_FinishedWatching();
    void on_ResetProgress();



};


#endif // SERIES_H
