#include "mainwindow.h"
#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

QT_USE_NAMESPACE
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    // Example use SerialPortInfo
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    qDebug() << "Name : " << info.portName();
    qDebug() << "Description : " << info.description();
    qDebug() << "Manufacturer: " << info.manufacturer();

    // Example use SerialPort
    QSerialPort serial;
    serial.setPort(info);
    if (serial.open(QIODevice::ReadWrite))
    serial.close();
    }

    return a.exec();
}
