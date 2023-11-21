#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QWidget>

namespace Ui {
class Overview;
}

class Overview : public QWidget
{
    Q_OBJECT

public:
    explicit Overview(QWidget *parent = nullptr);
    ~Overview();

    void AddSeriesToLayout(class Series* series);

protected:



private:
    Ui::Overview *ui;

    class MainWindow* m_MainWindow;
};

#endif // OVERVIEW_H
