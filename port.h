#ifndef PORT_H
#define PORT_H
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QtGui>
#include <QtCore>
#include <QDialog>
#include <QDirModel>
#include <QProgressBar>


class PotokPort : public QObject
{
   Q_OBJECT

public:

    QSerialPort thisPort;
    QString str;
    void run();
    bool flag=true;
    QByteArray Rbuf,Wbuf;



signals:
    void PrintMess(QString str);
    void PrintMess1(QString str);
    void PrintMess2(QByteArray str);
    void outPort(QString data);
    void TestRs485();
    void Green_lamp_232();
    void Green_lamp_485();
    void Red_lamp_232();
    void Red_lamp_485();
    void off_open();
    void on_open();
    void on_test();
    void off_test();
    void close_port_intest();
    void ping();
    void closeBtn();
    void saveReport();


public slots:
     void openPort(QString com, int baud, int parity);
     void closePort();
     void ReadInPort();
     void StartTest();
     void process_Port();

};

class GetMac: public QObject
{
    Q_OBJECT

public:
     void run();

public slots:
     void MacAdd(QString dir);
signals:
     void add(QString mac1,QString mac2,QString ip);
};

class GetComName: public QObject
{
    Q_OBJECT

public:
     void run();

public slots:
     void ComFound();
signals:
     void AddCom(QString com1,QString com2);
};
#endif // PORT_H
