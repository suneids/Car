#include "bluetoothbridge.h"
#include "bluetoothbridge.h"
#include <QDebug>

BluetoothBridge::BluetoothBridge(const QString &macAddress, QObject *parent)
    : QObject(parent), macAddress(macAddress), socket(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this)),
    discoveryAgent(new QBluetoothDeviceDiscoveryAgent(this)), isConnected(false)
{
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BluetoothBridge::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BluetoothBridge::discoveryFinished);
    connect(socket, &QBluetoothSocket::connected, this, [=]() {
        isConnected = true;
        qDebug() << "Bluetooth connected to" << macAddress;
    });
    connect(socket, &QBluetoothSocket::disconnected, this, [=]() {
        isConnected = false;
        qDebug() << "Bluetooth disconnected";
    });
    connect(socket, &QBluetoothSocket::errorOccurred, this, [=](QBluetoothSocket::SocketError error) {
        lastError = socket->errorString();
        qDebug() << "Bluetooth error:" << lastError;
    });

    discoveryAgent->start();
}

void BluetoothBridge::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    if (device.address().toString() == macAddress) {
        qDebug() << "Device found:" << device.name() << device.address().toString();
        socket->connectToService(device.address(), QBluetoothUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB")));
        discoveryAgent->stop();
    }
}

void BluetoothBridge::discoveryFinished()
{
    qDebug() << "Device discovery finished";
    if (!isConnected && !lastError.isEmpty()) {
        lastError = "Устройство не найдено или подключение не удалось";
        qDebug() << lastError;
    }
}

bool BluetoothBridge::connectToDevice(const QString &mac)
{
    if (isConnected) {
        qDebug() << "Already connected";
        return true;
    }
    if (mac != macAddress) {
        lastError = "MAC-адрес не совпадает с заданным";
        qDebug() << lastError;
        return false;
    }
    discoveryAgent->start();
    return true;
}

bool BluetoothBridge::sendMessage(const QByteArray &data)
{
    if (!isConnected) {
        lastError = "Не подключено к устройству";
        qDebug() << lastError;
        return false;
    }
    qint64 bytesWritten = socket->write(data);
    if (bytesWritten == -1) {
        lastError = socket->errorString();
        qDebug() << lastError;
        return false;
    }
    qDebug() << "Отправлено" << bytesWritten << "байт";
        return true;
}

void BluetoothBridge::disconnect()
{
    if (isConnected) {
        socket->disconnectFromService();
        isConnected = false;
        qDebug() << "Bluetooth отключён";
    }
}

QString BluetoothBridge::errorString() const
{
    return lastError;
}
//BluetoothBridge::BluetoothBridge(QObject *parent)
//    : QObject(parent)
//{
//    javaBridge = QJniObject("org/qtproject/example/CarControl/BluetoothBridge", "()V");
//    if (!javaBridge.isValid()) {
//        lastError = "Не удалось создать Java bridge";
//        qDebug() << lastError;
//    } else {
//        qDebug() << "Java bridge создан успешно";
//    }
//}

//bool BluetoothBridge::connectToDevice(const QString &mac){
//    bool result = javaBridge.callMethod<jboolean>(
//        "connect", "(Ljava/lang/String;)Z",
//        QJniObject::fromString(mac).object<jstring>()
//        );
//    qDebug() << "Connection resut: " << result;
//    return result;
//}
//\
//bool BluetoothBridge::sendMessage(const QByteArray &msg){
//    bool result = javaBridge.callMethod<jboolean>(
//        "send",
//        "(Ljava/lang/String;)Z",
//        QJniObject::fromString(msg).object<jstring>()
//        );
//    qDebug() << "Send result: " << result;
//    return result;
//}

//void BluetoothBridge::disconnect(){
//    javaBridge.callMethod<void>("disconnect", "()V");
//}
