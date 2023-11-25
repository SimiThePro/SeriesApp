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


}

Library::~Library()
{
    delete ui;
}

void Library::AddSeriesToLayout(Series* newSeries)
{
    ui->gridLayout->addWidget(newSeries);
}

void Library::SeriesPressed(Series *pressedSeries)
{


    m_MainWindow->SeriesPressed(pressedSeries);
}
