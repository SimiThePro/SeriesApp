#ifndef SERIES_H
#define SERIES_H

#include "qurl.h"
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
protected:

private slots:
    void on_pushButton_pressed();

private:
    Ui::Series *ui;

    class MainWindow* m_MainWindow;

    QString SeriesIconPath;
    QString SeriesPath;
    QString SeriesName;

    QList<class Section*> m_Sections;

    int m_SectionNumberIndex = 0;

    void SetButtonIcon(const QString& filename);
};

class Section{
public:
    Section(class QDir dir);

    QStringList getVideoFiles() const {return m_VideoFiles;}

    QString getSectionName() const {return m_SectionName;}


private:

    QString m_SectionName;
    QStringList m_VideoFiles;

};


#endif // SERIES_H
