QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addrelativeforworker.cpp \
    addworker.cpp \
    editworkerinfo.cpp \
    main.cpp \
    mainwindow.cpp \
    mytool.cpp \
    nodataindatafile.cpp \
    noopendatabase.cpp \
    queryrelativesform.cpp \
    starpointform.cpp

HEADERS += \
    addrelativeforworker.h \
    addworker.h \
    editworkerinfo.h \
    mainwindow.h \
    mytool.h \
    nodataindatafile.h \
    noopendatabase.h \
    queryrelativesform.h \
    starpointform.h

FORMS += \
    addrelativeforworker.ui \
    addworker.ui \
    editworkerinfo.ui \
    mainwindow.ui \
    nodataindatafile.ui \
    noopendatabase.ui \
    queryrelativesform.ui \
    starpointform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    WorkersData \
    testfile
