#include "addseries.h"

#include "SeriesOverview.h"
#include "mainwindow.h"
#include "overview.h"
#include "ui_addseries.h"

#include <QErrorMessage>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <Series.h>

AddSeries::AddSeries(QWidget *parent, Series* SeriesToEdit) :
    QDialog(parent),
    ui(new Ui::AddSeries)
{
    ui->setupUi(this);

    m_MainWindow = qobject_cast<MainWindow*>(parent);

    ui->SeriesNameLE->setPlaceholderText("Name");
    ui->SeriesPathLE->setPlaceholderText("Path");
    ui->ImageLE->setPlaceholderText("Image Path");

    if (SeriesToEdit == nullptr){
        m_Series = new Series(parent);
        bEdited = false;
    }else {
        bEdited = true;
        m_Series = SeriesToEdit;
        ui->SeriesNameLE->setText(m_Series->getSeriesName());
        ui->SeriesPathLE->setText(m_Series->getSeriesPath());
        ui->SeriesPathLE->setEnabled(false);
        ui->SeriesPathButton->setEnabled(false);
        ui->ImageLE->setText(m_Series->getSeriesIconPath());
    }


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
    if (!bEdited)
        m_Series->setSeriesPath(SeriesPath);


    QString ImagePath = ui->ImageLE->text();
    if (!ImagePath.isEmpty()){
        if (ImagePath.at(0) == '/'){
            ImagePath = ImagePath.sliced(1,ImagePath.size()-1);
        }
        m_Series->setSeriesIconPath(ImagePath);
    }

    m_Series->MainWindowParent(m_MainWindow);

    if (!bEdited){
        QFile file(static_cast<QString>(PROJECT_PATH) + "Data/Data.json");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for reading:" << file.errorString();
        }

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
        if (doc.isNull() || !doc.isArray()) {
            qDebug() << "Failed when creating:" << parseError.errorString();
            if (parseError.error != QJsonParseError::IllegalValue &&
                parseError.error != QJsonParseError::GarbageAtEnd){
                return;
            }
        }
        jsonData.clear();

        QJsonArray arr = doc.array();


        for (const auto& series : arr){
            QJsonObject seriesObject = series.toObject();

            if (seriesObject.contains("SeriesPath")){
                if (seriesObject["SeriesPath"].toString() == SeriesPath){
                    QErrorMessage* Error = new QErrorMessage(this);
                    Error->showMessage("This Series already exists");
                    return;
                }
            }
        }
        m_MainWindow->AddSeries(m_Series);
    }
    else{
        m_MainWindow->SimpleUpdateSeries(m_Series);
    }


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

