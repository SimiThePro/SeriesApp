#ifndef EPISODE_H
#define EPISODE_H

#include <QFileInfo>
#include <QJsonObject>
#include <QWidget>

namespace Ui {
class Episode;
}

class Episode : public QWidget
{

    Q_OBJECT
public:
    Episode(const QString& FilePath, class MainWindow* mainWindow);
    Episode(const QString& FilePath, int progress, int duration, MainWindow* mainWindow);
    ~Episode();

    QString getFilePath() const {return m_FileInfo.filePath();}

    void setDuration(int newDuration) {m_duration = newDuration;}
    int getDuration() const {return m_duration;}

    void setProgess(int newProgress);
    int getProgress() const {return m_progress;}

    void setProgressBar(class QProgressBar* newProgBar){m_ProgressBar= newProgBar;}
    QProgressBar* getProgressBar() const {return m_ProgressBar;}

    QString getEpisodeName() const {return m_EpisodeName;}




    void setJsonObject(const QJsonObject& newObject) {m_jsonObject = newObject;};
    void setSection(class Section* section) {m_Section = section;}
    Section* getSection() const {return m_Section;}

    void UpdateValue();

private:
    Ui::Episode *ui;

    MainWindow* m_MainWindow;

    class Section* m_Section;

    QString m_EpisodeName;

    QFileInfo m_FileInfo;
    int m_duration;
    int m_progress;

    QJsonObject m_jsonObject;

    void InitializeUi();

    class QProgressBar* m_ProgressBar;
};

#endif // EPISODE_H
