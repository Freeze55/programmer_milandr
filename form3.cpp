#include "form3.h"
#include "ui_form3.h"
#include <QStringListModel>
#include <QDebug>

form3::form3(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::form3)
{
    ui->setupUi(this);
}

form3::~form3()
{
    delete ui;
}


void form3::memo(QString s)
{
    ui->plainTextEdit->clear();
    ui->plainTextEdit->insertPlainText(s);
    ui->plainTextEdit->setReadOnly(true);

}
