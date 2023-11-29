#ifndef SERIES_H
#define SERIES_H

#include "qurl.h"
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QWidget>



class QFileInfo;
namespace Ui {
class Series;
}

class Series : public QWidget
{
    Q_OBJECT

public:
    explicit Series(QWidget *parent = nullptr);
    Series(QString SeriesPath, QString SeriesIconPath, QString SeriesName = "", QWidget *parent = nullptr);
    Series(const QString& SeriesName, const QString& SeriesPath, const QString& IconPath,QList<class Section*> sections, QWidget *parent = nullptr);
    ~Series();

    void setButtonImage(const QString& filename);
    QString getSeriesIconPath() const;
    void setSeriesIconPath(const QString &newSeriesIconPath);

    QString getSeriesPath() const;
    void setSeriesPath(const QString &newSeriesPath);

    QString getSeriesName() const;
    void setSeriesName(const QString &newSeriesName);

    QString PrintFiles();

    QList<class Section*> getSections() const {return m_Sections;}

    void MainWindowParent(class MainWindow* MW) {this->m_MainWindow = MW;}

    void SaveToFile();
protected:

private slots:
    void on_pushButton_pressed();

private:
    Ui::Series *ui;

    class MainWindow* m_MainWindow;

    QString m_SeriesIconPath;
    QString m_SeriesPath;
    QString m_SeriesName;

    QList<class Section*> m_Sections;

    int m_SectionNumberIndex = 0;

    void SetButtonIcon(const QString& filename);


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



private slots:
    void buttonPressed(class QAbstractButton *button);
};

//----------EPISODE----------

class Episode : public QObject{

    Q_OBJECT
public:
    Episode(const QString& FilePath, class MainWindow* mainWindow);
    Episode(const QString& FilePath, int progress, int duration, MainWindow* mainWindow);

    QString getFilePath() const {return m_FileInfo.filePath();}
    int getDuration() const {return m_duration;}
    int getProgress() const {return m_progress;}
    QString getEpisodeName() const {return m_EpisodeName;}


    void setDuration(int newDuration) {m_duration = newDuration;}

    void setJsonObject(const QJsonObject& newObject) {m_jsonObject = newObject;};
    void setSection(Section* section) {m_Section = section;}

    void UpdateValue();

private:

    MainWindow* m_MainWindow;

    class Section* m_Section;

    QString m_EpisodeName;

    QFileInfo m_FileInfo;
    int m_duration;
    int m_progress;

    QJsonObject m_jsonObject;


};

#endif // SERIES_H
