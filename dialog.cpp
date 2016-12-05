#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QTime>
#include <QThread>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    qDebug()<<"<good";
    if (ui->lineEdit->text()=="")
    {
        QMessageBox msgBox;
        msgBox.setText("Фамилия не введена!");
        msgBox.exec();
        qDebug()<<ui->lineEdit->text();
        //return;
    }
    else
    {

        this->close();
        QString str;
        str = "Фамилия: " + ui->lineEdit->text();

        print(str);
        qDebug()<<ui->lineEdit->text();
    }
}

void Dialog::on_pushButton_2_clicked()
{
     qDebug()<<"<good";qDebug()<<"<good";
     if (ui->lineEdit->text()=="")
     {
         QMessageBox msgBox;
         msgBox.setText("Фамилия не введена!");
         msgBox.exec();
         qDebug()<<ui->lineEdit->text();
         //return;
     }
     else
     {

         this->close();
         QString str;
         str = "Фамилия: " + ui->lineEdit->text();

        PrintStr(str);
        print(str);
         qDebug()<<ui->lineEdit->text();
     }
}
