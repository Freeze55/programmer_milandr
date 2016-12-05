#ifndef FORM2_H
#define FORM2_H

#include <QDialog>
#include <QtSql>
#include "form3.h"
namespace Ui {
class form2;
}

class form2 : public QDialog
{
    Q_OBJECT

public:
    explicit form2(QWidget *parent = 0);
    ~form2();
    QString Auditor;
    QString Name_pk;
    QString LoginDB;
    QString PassDB;
    QString AdressDB;
    QString NameDB;


private slots:
     void run();
     void open_list();
     void setParam( QString log, QString pass, QString adr, QString name);
signals:
    void set(QString s);
private:
    Ui::form2 *ui;
    QSqlDatabase db ;
};


#endif // FORM2_H
