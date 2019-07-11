#-------------------------------------------------
#
# Project created by QtCreator 2014-01-22T15:15:18
#
#-------------------------------------------------


QT       += core gui sql dbus
greaterThan(QT_MAJOR_VERSION, 4): QT +=widgets


win32{
QT       +=winextras
 QT+=multimedia
}else{
HEADERS  += notificationutill.h \
QT       +=dbus
 QT+=multimedia
QT+=x11extras
}

TEMPLATE = app
TARGET = elokab-adhan
DESTDIR = ../usr/bin

CONFIG += qt \
          release
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
INCLUDEPATH +=build
 RC_FILE = myapp.rc

SOURCES += main.cpp\
    database.cpp \
    location.cpp \
    manualcalc.cpp \
    display.cpp \
    adhan.cpp \
    dialogconfig.cpp \
    adhkarform.cpp \
    databaseadhkar.cpp \
   buttoncolor/buttoncolor.cpp \
    notification.cpp \
    colorsform.cpp \
mediaplayer.cpp



HEADERS  += \
    prayertimes.hpp \
    database.h \
    location.h \
    manualcalc.h \
    display.h \
    notification.h \
    adhan.h \
    dialogconfig.h \
    adhkarform.h \
    databaseadhkar.h \
    buttoncolor/buttoncolor.h \
    colorsform.h \
mediaplayer.h
#


FORMS    += \
    locationform.ui \
    manualcalcform.ui \
    displayform.ui \
    notificationform.ui \
    adhan.ui \
    dialogconfig.ui \
    adhkarform.ui \
    colorsform.ui
DEFINES += USE_MEDIA
CONFIG +=USE_MEDIA
PKGCONFIG-=multimedia

#packagesExist(multimedia)
#  {


#  }


 HEADERS  +=
  SOURCES +=


TRANSLATIONS    =../usr/share/elokab/translations/ar/elokab-adhan.ts\
                           ../usr/share/elokab/translations/fr/elokab-adhan.ts\
                           ../usr/share/elokab/translations/en/elokab-adhan.ts\


RESOURCES += \
    images.qrc

#INSTALL Linux
 target.path = /usr/bin
 data.files=../usr/share/*
 data.path=/usr/share
 INSTALLS +=    target \
                          data
             
