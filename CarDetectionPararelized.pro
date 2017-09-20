#-------------------------------------------------
#
# Project created by QtCreator 2017-08-30T22:15:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CarDetectionPararelized
TEMPLATE = app

SOURCES += \
        main.cpp \
    processedframe.cpp \
    framethread.cpp \
    framemanager.cpp \
    controller.cpp \
    objectdetector.cpp \
    lane.cpp \
    lanethread.cpp \
    contours.cpp

HEADERS += \
    processedframe.h \
    framethread.h \
    framemanager.h \
    controller.h \
    objectdetector.h \
    lane.h \
    lanethread.h \
    contours.h

INCLUDEPATH += /usr/include/opencv2
INCLUDEPATH += /usr/local/include/opencv2
LIBS += -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lopencv_highgui -lopencv_video -lopencv_imgproc -lopencv_objdetect
