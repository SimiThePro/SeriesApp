#include "addseries.h"
#include "mainwindow.h"
#include "overview.h"
#include "ui_addseries.h"

#include <QErrorMessage>
#include <QFileDialog>
#include <Series.h>

AddSeries::AddSeries(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSeries)
{
    ui->setupUi(this);

    m_MainWindow = qobject_cast<MainWindow*>(parent);

    m_Series = new Series(parent);

    ui->SeriesNameLE->setPlaceholderText("Name");
    ui->SeriesPathLE->setPlaceholderText("Path");
    ui->ImageLE->setPlaceholderText("Image Path");
}

AddSeries::~AddSeries()
{
    delete ui;
}

void AddSeries::on_SeriesPathButton_pressed()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Series"));
    fileDialog.setFileMode(QFileDialog::Directory);
    if (fileDialog.exec() == QDialog::Accepted)
        ui->SeriesPathLE->setText(fileDialog.selectedUrls().constFirst().path());
}


void AddSeries::on_buttonBox_accepted()
{

    if (!QUrl::fromLocalFile(ui->SeriesPathLE->text()).isValid()) {
        QErrorMessage* Error = new QErrorMessage(this);
        Error->showMessage("The File is not Valid");
        return;
    }

    m_Series->setSeriesName(ui->SeriesNameLE->text());
    QString SeriesPath = ui->SeriesPathLE->text();
    if (SeriesPath.at(0) == '/'){
        SeriesPath = SeriesPath.sliced(1,SeriesPath.size()-1);
    }
    m_Series->setSeriesPath(SeriesPath);
    QString ImagePath = ui->ImageLE->text();
    if (!ImagePath.isEmpty()){
        if (ImagePath.at(0) == '/'){
            ImagePath = ImagePath.sliced(1,ImagePath.size()-1);
        }
        m_Series->setSeriesIconPath(ImagePath);
    }

    m_Series->MainWindowParent(m_MainWindow);

    m_MainWindow->AddSeries(m_Series);
}


void AddSeries::on_ImageButton_pressed()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Series"));
    fileDialog.setFileMode(QFileDialog::AnyFile);
    if (fileDialog.exec() == QDialog::Accepted)
        ui->ImageLE->setText(fileDialog.selectedUrls().constFirst().path());
}

