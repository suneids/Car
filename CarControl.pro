QT       += core gui bluetooth serialport
android {
    QT += androidextras
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bluetoothbridge.cpp \
    joystick.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    bluetoothbridge.h \
    joystick.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

ANDROID_PACKAGE_SOURCE_DIR = C:/Users/user/AndroidStudioProjects/BluetoothBridge/app/src/main

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
