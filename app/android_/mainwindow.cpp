#include "mainwindow.h"
#include "ui_mainwindow.h"
char startByte = 0xFE,
    endByte = 0xFF;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    joyStick = new Joystick(ui->HandleFrame);
    joyStick->raise();
    joyStick->show();
    QString mac = "98:DA:50:01:E9:2A";
    bluetoothBridge = new BluetoothBridge("98:DA:50:01:E9:2A", this);

    tim = new QTimer(this);
    connect(tim, &QTimer::timeout, this, &MainWindow::onTimeout);
    tim->start(6); // или 10, 2 - мало, 100 - много
}

MainWindow::~MainWindow()
{
    delete ui;
    delete joyStick;
}


void MainWindow::onTimeout(){
    QByteArray packet = calculateInfo();
    bluetoothBridge->sendMessage(packet);
//    serialPort->write(packet);
//    if(serialPort->isOpen()){
//        bytesWritten = serialPort->write(packet);
//    }

}


 QByteArray MainWindow::calculateInfo(){
    QByteArray result;
    int dx = joyStick->dx,
        dy = joyStick->dy;
    signed char speed = std::sqrt(dx*dx+dy*dy);
    double angle = qAtan2(abs(dy), dx);
    unsigned char deg = qRadiansToDegrees(angle);
    dy = -dy;//Инвертируем ось, теперь вверх это + изменение

    qDebug() << deg;

    if(dx == dy && dx == 0){
        deg = 90;
    }
    else{
        deg -= 180; //меняем точку отсчета, теперь самая левая точка не 180° а 0°
        deg = -deg; //инвертируем угол, теперь он увеличивается по часовой стрелке.
        if(dy < 0){
            deg -=180;// в случае движения назад смещаем точку отсчета еще на половину окружности, теперь 0° на своем привычном месте, но там где 270° теперь 90°.
            deg = 180 - deg;
        }
    }
    if(dy < 0){
        speed = -speed; //Скорость потеряла свой знак, в случае джойстика внизу, делаем ее отрицательной
    }
    result.append(startByte);
    result.append(deg);
    result.append(speed);
    qDebug() << deg << speed;
    result.append(deg ^ speed);
    return result;
}
