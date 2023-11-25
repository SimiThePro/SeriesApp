#ifndef LIBRARY_H
#define LIBRARY_H

#include <QWidget>

namespace Ui {
class Library;
}

class Library : public QWidget
{
    Q_OBJECT

public:
    explicit Library(QWidget *parent = nullptr);
    ~Library();

    void AddSeriesToLayout(class Series* newSeries);

    void SeriesPressed(Series* pressedSeries);

private:
    Ui::Library *ui;

    int m_countElementsWidth, m_countElementsHeight;

    class MainWindow* m_MainWindow;
};

#endif // LIBRARY_H
