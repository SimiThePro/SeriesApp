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
    int getDuration() const {return m_duration;}
    int getProgress() const {return m_progress;}
    QString getEpisodeName() const {return m_EpisodeName;}


    void setDuration(int newDuration) {m_duration = newDuration;}

    void setJsonObject(const QJsonObject& newObject) {m_jsonObject = newObject;};
    void setSection(class Section* section) {m_Section = section;}

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

};

#endif // EPISODE_H
