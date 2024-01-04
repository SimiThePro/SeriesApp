#include "library.h"
#include "mainwindow.h"
#include "ui_library.h"

#include <Series.h>

Library::Library(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Library)
{
    ui->setupUi(this);

    m_MainWindow = qobject_cast<MainWindow*>(parent);

    if (m_MainWindow == nullptr) qInfo() << "Error";

    ui->comboBox->addItem("Test");
    ui->comboBox->addItem("Ben");

    MaxRowElements = 3;
}

Library::~Library()
{
    delete ui;
}

void Library::AddSeriesToLayout(Series* newSeries)
{
    int count = ui->gridLayout->count();
    int row,column;

    if (count != 0){
        row = (std::floor(count / MaxRowElements));
        column = (count % MaxRowElements);
    }else {
        row = 0;
        column = 0;
    }


    ui->gridLayout->addWidget(newSeries,row,column,Qt::AlignLeft);
}

void Library::SeriesPressed(Series *pressedSeries)
{
    m_MainWindow->SeriesPressed(pressedSeries);
}
