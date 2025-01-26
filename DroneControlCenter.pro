QT       += core gui 3dcore 3drender 3dinput 3dextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Anotc/anotc.cpp \
    Anotc/anotc_json.cpp \
    Widgets/frametable.cpp \
    Widgets/serialportworker.cpp \
    Widgets/dronemodel.cpp \
    Widgets/serialportpanel.cpp \
    Widgets/udppanel.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Anotc/anotc.h \
    Anotc/anotc_json.h \
    Widgets/frametable.h \
    Widgets/serialportworker.h \
    Widgets/dronemodel.h \
    Widgets/serialportpanel.h \
    Widgets/udppanel.h \
    mainwindow.h

FORMS += \
    Widgets/dronemodel.ui \
    Widgets/frametable.ui \
    Widgets/serialportpanel.ui \
    Widgets/udppanel.ui \
    mainwindow.ui

QT += serialport charts network

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

macx {
QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
}

RESOURCES += \
    mysource.qrc

DISTFILES += \
    FrameDefination.json
