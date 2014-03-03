#-------------------------------------------------
#
# Project created by QtCreator 2015-04-07T14:48:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClassifieurSVM_01
TEMPLATE = app


SOURCES += main.cpp\
        mysvm.cpp

HEADERS  += mysvm.h

FORMS    += mysvm.ui



INCLUDEPATH+= D:\\openCV\\build\\include

LIBS += -LD:\\openCV\\x64\\lib\\Release \
            -lopencv_calib3d2411 \
            -lopencv_contrib2411 \
            -lopencv_core2411 \
            -lopencv_features2d2411 \
            -lopencv_flann2411 \
            -lopencv_gpu2411 \
            -lopencv_highgui2411 \
            -lopencv_imgproc2411 \
            -lopencv_legacy2411 \
            -lopencv_ml2411 \
            -lopencv_nonfree2411 \
            -lopencv_objdetect2411 \
            -lopencv_ocl2411 \
            -lopencv_photo2411 \
            -lopencv_stitching2411 \
            -lopencv_superres2411 \
            -lopencv_video2411 \
            -lopencv_videostab2411
