#ifndef BLUETOOTHBRIDGE_H
#define BLUETOOTHBRIDGE_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>

class BluetoothBridge : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothBridge(const QString &macAddress, QObject *parent = nullptr);
    bool connectToDevice(const QString &mac);
    bool sendMessage(const QByteArray &data);
    void disconnect();
    QString errorString() const;

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void discoveryFinished();

private:
    QString macAddress;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothSocket *socket;
    QString lastError;
    bool isConnected;
};
#endif
