QT       += core gui 3dcore 3drender 3dinput 3dextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Anotc/anotc.cpp \
    Anotc/anotc_cmd_frame.cpp \
    Anotc/anotc_config_frame.cpp \
    Anotc/anotc_custom_frame.cpp \
    Anotc/anotc_data_frame.cpp \
    Anotc/anotc_receive_check.cpp \
    Anotc/anotcthread.cpp \
    Anotc/blockingqueue.cpp \
    DLog.cpp \
    Widgets/calibrationform.cpp \
    Widgets/dataanalysicschart.cpp \
    Widgets/datachartform.cpp \
    Widgets/datatable.cpp \
    Widgets/mixerform.cpp \
    Widgets/parameterform.cpp \
    Widgets/serialportworker.cpp \
    Widgets/dronemodel.cpp \
    Widgets/serialportpanel.cpp \
    Widgets/udppanel.cpp \
    main.cpp \
    mainwindow.cpp \
    udpthread.cpp

HEADERS += \
    Anotc/anotc.h \
    Anotc/anotc_cmd_frame.h \
    Anotc/anotc_config_frame.h \
    Anotc/anotc_custom_frame.h \
    Anotc/anotc_data_frame.h \
    Anotc/anotc_receive_check.h \
    Anotc/anotcthread.h \
    Anotc/blockingqueue.h \
    DLog.h \
    Widgets/calibrationform.h \
    Widgets/dataanalysicschart.h \
    Widgets/datachartform.h \
    Widgets/datatable.h \
    Widgets/mixerform.h \
    Widgets/parameterform.h \
    Widgets/serialportworker.h \
    Widgets/dronemodel.h \
    Widgets/serialportpanel.h \
    Widgets/udppanel.h \
    mainwindow.h \
    udpthread.h

FORMS += \
    Widgets/calibrationform.ui \
    Widgets/datachartform.ui \
    Widgets/datatable.ui \
    Widgets/dronemodel.ui \
    Widgets/mixerform.ui \
    Widgets/parameterform.ui \
    Widgets/serialportpanel.ui \
    Widgets/udppanel.ui \
    mainwindow.ui

QT += serialport charts network 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation

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
    DataFrameDefination.json
