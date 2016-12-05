#include "port.h"
#include <QString>
#include <QMessageBox>
void PotokPort::process_Port()
{
connect(&thisPort, SIGNAL(readyRead()),this,SLOT(ReadInPort()));
}
void PotokPort::openPort(QString com,int baud,int parity)
{
    thisPort.setPortName(com);                       //параметры ком порта 232
    thisPort.setBaudRate((QSerialPort::BaudRate) baud);
    thisPort.setDataBits(QSerialPort::Data8);
    thisPort.setParity((QSerialPort::Parity) parity);
    thisPort.setStopBits(QSerialPort::OneStop);
    thisPort.setFlowControl(QSerialPort::NoFlowControl);
    thisPort.open(QIODevice::ReadWrite);
    if (thisPort.isOpen())
        {

        str= "Порт открыт "+com+" c параметрами:";
        PrintMess(str);
        str= "Cкорость: "+ QString::number(baud);
        PrintMess(str);
        str= "Четность: "+ QString::number(parity);
        PrintMess(str);
        Green_lamp_232();
        on_open();
        }
    else
    {
        str="Ошибка открытия COM порта "+ com;
        PrintMess(str);
        off_open();
        closeBtn();
        off_test();
        return;
    }

}

void PotokPort::closePort()
{
    if(thisPort.isOpen())
    {
        thisPort.close();
        str= "Порт закрыт ";
        PrintMess(str);
        Red_lamp_232();
        off_open();
    }

}
void PotokPort:: ReadInPort()
{
    QString data;

    if (flag==true)
    {
    data.append(thisPort.readAll());

    if (data!="\r");
        outPort(data);

    if (data == "TEST_RS485:\r")
        TestRs485();

    if ((data == "BATTERY_ERR!\r===END_TEST===\r")||(data == "BATTERY_OK!\r===END_TEST===\r"))
    {
        thisPort.close();
        str= "Тест завершен";
        PrintMess(str);
        Red_lamp_232();
        off_open();
        saveReport();



        return;

    }
    qDebug() << data;

    }

}
void PotokPort::StartTest()
{
    if (thisPort.isOpen())
    {
    flag=false;
    str = "Запуск теста...";  //открыли ком и отправили команду
    PrintMess1(str);
    Rbuf.clear();
    Wbuf.clear();
    Wbuf[0]='R';
    thisPort.write(Wbuf,1);
    qDebug() << "SEND";
    qDebug() << Wbuf;
    qDebug() << Wbuf.size();
    thisPort.flush();
    thisPort.waitForReadyRead(1000);
    Rbuf = thisPort.readAll();
    qDebug() << "Resive!";
    qDebug() << Rbuf;
    qDebug() << Rbuf.size();

    if ((Rbuf.size() != 1)||(Rbuf[0]!='R'))
    {
        str = "\rОшибка обмена ";
        PrintMess(str);
        off_test();
        close_port_intest();
        flag = true;
        return;
    }
    else
    {
        str = " OK";
        PrintMess(str);
        ping();

    }
    flag = true;

    }
    else return;
}
// ////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GetMac::MacAdd(QString dir)
{
    QFile file(dir);
    QString line1,line2,line3,tmp,tmp2,tmp3,mac1,mac2,ip;
    int i = 0;



    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream t(&file);
        while(!t.atEnd())
        {
            line1 = t.readLine();

            i++;
        }



    }
    file.close();
    if (line1.size() == 11)
    {
        file.open(QIODevice::ReadOnly);
        QTextStream t(&file);
        for (int j=1;i>j;j++)
        {
            line2 = t.readLine();


        }
        file.close();
        line3 = line2+line1;

    }
    if((line1.size()==54)||(line3.size()==54))
    {
            if(line1.size()==54)
                tmp = line1;
            else tmp = line3;

            mac1[0]= tmp[9];   mac2[0]=tmp[21];
            mac1[1]= tmp[10];  mac2[1]=tmp[22];
            mac1 += ":";       mac2 +=";";
            mac1[3]= tmp[11];  mac2[3]=tmp[23];
            mac1[4]= tmp[12];  mac2[4]=tmp[24];
            mac1 += ":";       mac2 +=";";
            mac1[6]= tmp[13];  mac2[6]=tmp[25];
            mac1[7]= tmp[14];  mac2[7]=tmp[26];
            mac1 += ":";       mac2 +=";";
            mac1[9]= tmp[15];  mac2[9]=tmp[27];
            mac1[10]= tmp[16]; mac2[10]=tmp[28];
            mac1 += ":";       mac2 +=";";
            mac1[12]= tmp[17]; mac2[12]=tmp[29];
            mac1[13]= tmp[18]; mac2[13]=tmp[30];
            mac1 += ":";       mac2 +=";";
            mac1[15]= tmp[19]; mac2[15]=tmp[31];
            mac1[16]= tmp[20]; mac2[16]=tmp[32];




           bool ok;
           tmp2.clear();

           tmp2[0] = tmp[33];
           tmp2[1] = tmp[34];
           int hex =  tmp2.toInt(&ok,16);

           tmp3+=QString::number(hex);

           tmp3+=".";
           tmp2[0] = tmp[35];
           tmp2[1] = tmp[36];
           hex =  tmp2.toInt(&ok,16);

           tmp3+=QString::number(hex);

           tmp3+=".";
           tmp2[0] = tmp[37];
           tmp2[1] = tmp[38];
           hex =  tmp2.toInt(&ok,16);

           tmp3+=QString::number(hex);

           tmp3+=".";
           tmp2[0] = tmp[39];
           tmp2[1] = tmp[40];
           hex =  tmp2.toInt(&ok,16);

           tmp3+=QString::number(hex);





            ip=tmp3;


            add(mac1,mac2,ip);


    }
    else
    {
        mac1="";
        mac2="";
        ip="";
        add(mac1,mac2,ip);
    }

}

// /////////////////////////////////////////////////////////////////////////////
// /// ///////////////////////////////////////////////////////////////////////
void GetComName::ComFound()
{
    QString com1,com2;
    int p = 1;
    while(p=1){
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
        com1=info.portName();
        com2=info.portName();
        AddCom(com1,com2);
        }


    Sleep(10000);
    }

}
