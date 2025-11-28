#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "joystick.h"
#include "bluetoothbridge.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void onTimeout();
public:
    MainWindow(QWidget *parent = nullptr);
    QByteArray calculateInfo();
    ~MainWindow();
    Joystick *joyStick = nullptr;
    QTimer *tim = nullptr;
    QSerialPort *serialPort = nullptr;
    BluetoothBridge *bluetoothBridge = nullptr;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
