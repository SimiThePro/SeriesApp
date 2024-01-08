QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += PROJECT_PATH=\"\\\"$${_PRO_FILE_PWD_}/\\\"\"

SOURCES += \
    addseries.cpp \
    custompushbutton.cpp \
    episode.cpp \
    library.cpp \
    main.cpp \
    mainwindow.cpp \
    overview.cpp \
    series.cpp \
    seriesoverview.cpp \
    seriespreview.cpp

HEADERS += \
    addseries.h \
    custompushbutton.h \
    episode.h \
    library.h \
    mainwindow.h \
    overview.h \
    series.h \
    seriesoverview.h \
    seriespreview.h

FORMS += \
    SeriesPreview.ui \
    addseries.ui \
    episode.ui \
    library.ui \
    mainwindow.ui \
    overview.ui \
    series.ui \
    seriesoverview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Test.qrc
