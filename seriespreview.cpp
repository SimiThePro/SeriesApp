#include "seriespreview.h"
#include "episode.h"
#include "qevent.h"

#include <CustomPushButton.h>
#include <MainWindow.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QStackedLayout>
#include <Series.h>

SeriesPreview::SeriesPreview(Series* series, MainWindow* mainWindow,QWidget *parent)
    : QWidget{parent},m_Series(series),m_MainWindow(mainWindow)
{

    m_lastWatchedEpisode = nullptr;



    QWidget* mainwidget = new QWidget(this);

    QWidget* back = new QWidget(this);

    QVBoxLayout * bckgrnd = new QVBoxLayout(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_lastWatchedEpisode = m_Series->getEpisodeFromFilePath(m_Series->getPathLastEpisodeWatched());

    // Load the background image
    QPixmap backgroundImage(series->getSeriesIconPath());
    QLabel *backgroundLabel = new QLabel(mainwidget);
    backgroundLabel->setPixmap(backgroundImage);
    backgroundLabel->setScaledContents(true); // Ensure the pixmap scales to the label size
    backgroundLabel->lower();

    // Create the button and set it in the top right corner
    PlayButton = new QPushButton(mainwidget);
    PlayButton->setIcon(QIcon("C:/Users/simim/Downloads/free-play-1779827-1513991.png"));
    PlayButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(PlayButton,SIGNAL(pressed()),this,SLOT(on_PlayButton_pressed()));

    // Create the button and set it in the top right corner
    ExitButton = new QPushButton("X", mainwidget);
    ExitButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ExitButton->setMaximumSize(20,20);

    connect(ExitButton,SIGNAL(pressed()),this,SLOT(on_ExitButtonPressed()));

    Progress = new QProgressBar(mainwidget);
    Progress->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    Progress->setMaximum(m_lastWatchedEpisode->getDuration());
    Progress->setValue(m_lastWatchedEpisode->getProgress());
    Progress->setTextVisible(false);


    // Set up the layout
    bckgrnd->addWidget(backgroundLabel);

    mainLayout->addWidget(ExitButton,1, Qt::AlignRight | Qt::AlignTop);
    mainLayout->addWidget(PlayButton,2, Qt::AlignHCenter | Qt::AlignVCenter);
    mainLayout->addWidget(Progress,1,Qt::AlignBottom);


    QStackedLayout* sl = new QStackedLayout();

    mainwidget->setLayout(mainLayout);
    back->setLayout(bckgrnd);

    sl->addWidget(mainwidget);
    sl->addWidget(back);

    back->stackUnder(mainwidget);
    mainwidget->raise();

    sl->setStackingMode(QStackedLayout::StackAll);

    setLayout(sl);
    setMinimumSize(150,200);
    setMaximumSize(150,200);
}

void SeriesPreview::setLastWatchedEpisode(Episode *episode)
{
    m_lastWatchedEpisode = episode;
}



void SeriesPreview::UpdateProgressBar()
{
    Progress->setMaximum(m_lastWatchedEpisode->getDuration());
    Progress->setValue(m_lastWatchedEpisode->getProgress());
}

void SeriesPreview::on_PlayButton_pressed()
{
    m_lastWatchedEpisode = m_Series->getEpisodeFromFilePath(m_Series->getPathLastEpisodeWatched());
    if (m_lastWatchedEpisode == nullptr) return;

    m_MainWindow->EpisodeSelected(m_lastWatchedEpisode);
    m_MainWindow->setPressedSeriesPreview(this);
}

void SeriesPreview::on_ExitButtonPressed()
{

}

void SeriesPreview::on_BackgroundButton_pressed()
{
    qInfo() << "Backgroundbutton pressed";
}

void SeriesPreview::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton){
        if (!ExitButton->underMouse() && !PlayButton->underMouse()){
            m_MainWindow->SeriesPressed(m_Series);
        }
    }
}

void SeriesPreview::enterEvent(QEnterEvent *event)
{
    setStyleSheet(".QLabel{border: 2px solid black;}");
}

void SeriesPreview::leaveEvent(QEvent *event)
{
    setStyleSheet("");
}
