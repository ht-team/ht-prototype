#-------------------------------------------------
#
# Project created by QtCreator 2012-04-15T19:57:56
#
#-------------------------------------------------

QT       += core gui



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include("../treetotable/treetotable.pri")
include("../mt/mt.pri")

TARGET = example
TEMPLATE = app


SOURCES += main.cpp\
        mainform.cpp \
    exampletempateserrialization.cpp \
    comparetest.cpp

HEADERS  += mainform.h \
    exampletempateserrialization.h \
    comparetest.h

FORMS    += mainform.ui


