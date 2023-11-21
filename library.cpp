#include "library.h"
#include "ui_library.h"

#include <Series.h>

Library::Library(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Library)
{
    ui->setupUi(this);

    ui->comboBox->addItem("Niger");
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
