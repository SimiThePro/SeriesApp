#ifndef ADDSERIES_H
#define ADDSERIES_H

#include <QDialog>

namespace Ui {
class AddSeries;
}

class AddSeries : public QDialog
{
    Q_OBJECT

public:
    explicit AddSeries(QWidget *parent = nullptr, class Series* SeriesToEdit = nullptr);

    ~AddSeries();

private slots:
    void on_SeriesPathButton_pressed();

    void on_buttonBox_accepted();

    void on_ImageButton_pressed();

private:
    Ui::AddSeries *ui;

    class MainWindow* m_MainWindow;

    class Series* m_Series;
    bool bEdited;
};

#endif // ADDSERIES_H
