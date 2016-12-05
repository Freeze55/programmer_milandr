#include "form2.h"
#include "ui_form2.h"
#include <QMessageBox>
#include <QString>
#include <QDesktopWidget>
#include <QScreen>
#include <QMetaEnum>
#include <unistd.h>
#include <errno.h>
#include <QThread>
#include <QProcess>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QListView>
#include <QListWidget>
#include <QListWidgetItem>
#include <QByteArray>
#include <QProgressBar>
#include <QIcon>
#include <QtSql>
#include <QMessageBox>
form2::form2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::form2)
{
    ui->setupUi(this);


    QThread *Thread3 =new QThread();
    form3 * f3 = new form3;
    f3->moveToThread(Thread3);

    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(open_list()));
    connect(this,SIGNAL(set(QString)),f3,SLOT(memo(QString)));
    connect(this,SIGNAL(set(QString)),f3,SLOT(exec()));
    Thread3->start();


}

form2::~form2()
{
    delete ui;
}

void form2::setParam(QString log, QString pass, QString adr, QString name)
{
    LoginDB = log;
    PassDB = pass ;
    AdressDB = adr;
    NameDB = name;
}

void form2::run()
{

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(AdressDB);
    db.setDatabaseName(NameDB);
    db.setUserName(LoginDB);
    db.setPassword(PassDB);
    db.open();

    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("table2");
    model->select();



    ui->tableView->setModel(model);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->tableView->horizontalHeader()->resizeSection(7,0);

    ui->tableView->horizontalHeader()->resizeSection(2,40);
    ui->tableView->horizontalHeader()->resizeSection(1,35);
    ui->tableView->horizontalHeader()->resizeSection(0,0);



}
void form2::open_list()
{

    QString way;
    QModelIndex index;
    index = ui->tableView->model()->index(ui->tableView->currentIndex().row(),7);

    way=index.data().toString();

    set(way);





}
