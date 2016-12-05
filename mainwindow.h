#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QtGui>
#include <QtCore>
#include <QDialog>
#include <QDirModel>
#include <QProgressBar>
#include <QtSql>
#include "port.h"
#include "form2.h"
#include "form3.h"
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool erase,eraseAll,program,verify;
    bool flag = true;
    bool flag2 = true;
    QString Auditor;
    QString Name_pk;
    QString LoginDB;
    QString PassDB;
    QString AdressDB;
    QString NameDB;

signals:
    void StartRun(QString portName,QString dir);
    void CheckParam(bool e, bool eA, bool p, bool v);
    void GetIpMac(QString ip,QString mac1,QString mac2,QString dir);
    void StartOn (QString com );
    void TestOn(QString com1,QString com2);
    void open232(QString com,int baud,int parity);
    void close232();
    void setMac(QString dir);
    void BtnClose();
    void BtnOpen();
    void setDB(QString Login,QString Pass,QString Adress,QString Name );
    void rundb();




private slots:
    void Print(QString str);
    void Print1(QString str);
    void Print2(QString str);
    void on_RecordBtn_clicked();
    void on_ClearBtn_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_5_clicked();
    void setMaxPrBar(int value);
    void AddTooBar(int value);
    void setRrBarNull();
    void on_testBtn_clicked();
    void closeEvent(QCloseEvent *event);
    void on_portBtn_clicked();
    void clicked();
    void on_checkBox_clicked();
    void SetMac1Mac2Ip(QString mac1,QString mac2,QString ip);
    void findTEXT(QString com1,QString com2);
    void Green_lamp_232();
    void Green_lamp_485();
    void Red_lamp_232();
    void Red_lamp_485();
    void Btn_close();
    void Btn_open();
    void hex();
    void Refresh();
    void off_prg();
    void on_prg();
    void on_test();
    void off_test();
    void off_open();
    void on_open();
    void SaveRep();



    void on_getMacPromBD_clicked();

    void on_pushButton_clicked();






    void on_openReportsBtn_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db ;
    QDirModel *model;

};

class PotokThread : public QObject
{
   Q_OBJECT

public:

    bool erase,eraseAll,program,verify;
    void run();
    DWORD dwadrboot;
    DWORD dwadr_seg_hex,dwadr_lineoffs_hex;
    int ilboot,ilcod;
    BYTE bufram[0x8000];
    BYTE bufcod[0x20000];
    BYTE buf_hexstr[530];
    WORD wadr_offs_hex;
    BYTE bl_hex,btype_hex;
    BYTE buf_data_hex[256];
    QByteArray datafromport;
    BOOL GetBootCod(void);
    BOOL GetHexCod(QString dir);
    BYTE GetHexByte(int ibuf);
    BOOL GetDataHex(void);
    BOOL CharHexToBYTE(char str[2]);
    QString str;
    QString ComRs485; //буфер название порта
    PotokPort Port;
    int baud,parity;



signals:
    void PrintMess(QString str);
    void PrintMess1(QString str);
    void PrintMess2(QString str);
    void SetMaxBar(int max);
    void AddtoBar(int value);
    void setRrBarNull();
    void StartPort232(QString com,int baud,int parity);
    void ClosePort232();
    void StartTest();
    void Green_lamp_232();
    void Green_lamp_485();
    void Red_lamp_232();
    void Red_lamp_485();
    void off_prg();
    void on_prg();
    void on_test();
    void off_test();





public slots:
   void SetParam(bool e, bool eA, bool p, bool v);
   void SetIpMac(QString ip,QString mac1,QString mac2,QString dir);
   void Start(QString portName, QString dir);

   void StartTest(QString com1,QString com2 ) ;
   void TestRs485();
   void Ping();

};
#endif // MAINWINDOW_H
