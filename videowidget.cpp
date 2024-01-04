// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "videowidget.h"


#include <MainWindow.h>
#include <QApplication>
#include <QApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWindow>


VideoWidget::VideoWidget(QWidget *parent) : QVideoWidget(parent)
{
    m_MainWindow = qobject_cast<MainWindow*>(parent);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::black);
    setPalette(p);

    screens = qApp->screens();

    for (auto s : screens){
        qInfo() << s->name();
    }


#ifndef Q_OS_ANDROID // QTBUG-95723
    setAttribute(Qt::WA_OpaquePaintEvent);
#endif
}

void VideoWidget::SetFullscreen()
{
    setFullScreen(true);
}

void VideoWidget::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Escape || event->key() == Qt::Key_Back) && isFullScreen()) {
        setFullScreen(false);
        event->accept();
    } else if (event->key() == Qt::Key_Enter && event->modifiers() & Qt::Key_Alt) {
        setFullScreen(!isFullScreen());
        event->accept();
    } else {
        QVideoWidget::keyPressEvent(event);
    }
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{


    setFullScreen(!isFullScreen());

    return;

    if (isFullScreen()){
        setFullScreen(false);
        showNormal();
        //setGeometry(qApp->screens()[0]->availableGeometry());
    }else {
        setFullScreen(true);

        setGeometry(qApp->screens()[0]->availableGeometry());
    }


    return;


    m_MainWindow->windowHandle()->setScreen(qApp->screens()[1]);
    setFullScreen(!isFullScreen());

    return;
    m_MainWindow->MakeVideoPlayerFullscreen();

    return;

    QWidget * widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->addWidget(this);
    widget->windowHandle()->setScreen(qApp->screens()[1]);
    setFullScreen(true);


    //widget->windowHandle()->setScreen(qApp->screens()[0]);
    //widget->showFullScreen();

    /*

    setScreen(screens[0]);
    event->accept();
    */

}

void VideoWidget::mousePressEvent(QMouseEvent *event)
{
    QVideoWidget::mousePressEvent(event);
}

