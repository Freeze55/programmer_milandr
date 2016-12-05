#include "mainwindow.h"
#include "ui_mainwindow.h"
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



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    QThread *thread = new QThread();
    PotokThread *Potok = new PotokThread();
    Potok->moveToThread(thread);


    QThread *portThread = new QThread();
    PotokPort *Port =new PotokPort();
    Port->moveToThread(portThread);
    Port->thisPort.moveToThread(portThread);

    QThread *threadForMac = new QThread();
    GetMac *Mac = new GetMac();
    Mac->moveToThread(threadForMac);

    QThread *threadForCom =new QThread();
    GetComName *Com = new GetComName();
    Com->moveToThread(threadForCom);

    QThread *FormTwo =new QThread();
    form2 * f2 = new form2;
    f2->moveToThread(FormTwo);

    connect(this, SIGNAL(StartRun(QString,QString)), Potok, SLOT(Start(QString,QString)),Qt::QueuedConnection); // заливка(выполнить)
    connect(Potok, SIGNAL(PrintMess(QString)), this, SLOT(Print(QString)),Qt::QueuedConnection); //печать в консоль
    connect(Potok,SIGNAL(PrintMess2(QString)),this,SLOT(Print2(QString)),Qt::QueuedConnection);
    connect(Potok,SIGNAL(PrintMess1(QString)),this,SLOT(Print1(QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(CheckParam(bool,bool,bool,bool)),Potok,SLOT(SetParam(bool,bool,bool,bool)),Qt::QueuedConnection);//передача галочек в поток
    connect(this,SIGNAL(GetIpMac(QString,QString,QString,QString)),Potok,SLOT(SetIpMac(QString,QString,QString,QString)),Qt::QueuedConnection);//передача в поток маков и айпи
    connect(Potok,SIGNAL(SetMaxBar(int)),this,SLOT(setMaxPrBar(int)),Qt::QueuedConnection);//прогресс бар установка максимума
    connect(Potok,SIGNAL(AddtoBar(int)),this,SLOT(AddTooBar(int)),Qt::QueuedConnection);//изменение прогресса
    connect(Potok,SIGNAL(setRrBarNull()),this,SLOT(setRrBarNull()),Qt::QueuedConnection);
    connect(this,SIGNAL(StartOn(QString)),Potok,SLOT(GoOn(QString)),Qt::QueuedConnection); //кнопка запустить
    connect(this,SIGNAL(TestOn(QString,QString)),Potok,SLOT(StartTest(QString,QString)),Qt::QueuedConnection);//кнопка запустить тест

    connect(this,SIGNAL(open232(QString,int,int)),Port,SLOT(openPort(QString,int,int)),Qt::QueuedConnection);
    connect(this,SIGNAL(close232()),Port,SLOT(closePort()),Qt::QueuedConnection);
    connect(Port, SIGNAL(PrintMess(QString)), this, SLOT(Print(QString)),Qt::QueuedConnection);
    connect(Port, SIGNAL(PrintMess1(QString)), this, SLOT(Print1(QString)),Qt::QueuedConnection);//печать в консоль
    connect(Port, SIGNAL(PrintMess2(QString)), this, SLOT(Print2(QString)),Qt::QueuedConnection); //печать в консоль

    connect(Potok,SIGNAL(StartPort232(QString,int,int)),Port,SLOT(openPort(QString,int,int)),Qt::QueuedConnection);

    connect(Potok, SIGNAL(ClosePort232()),Port,SLOT(closePort()),Qt::QueuedConnection);
    connect(Potok, SIGNAL(StartTest()),Port,SLOT(StartTest()),Qt::QueuedConnection);

    connect(Port,SIGNAL(outPort(QString)),this,SLOT(Print2(QString)),Qt::QueuedConnection);


    connect(Port,SIGNAL(TestRs485()),Potok,SLOT(TestRs485()),Qt::QueuedConnection);

    connect(portThread, SIGNAL(started()), Port, SLOT(process_Port()),Qt::QueuedConnection);
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(clicked()),Qt::QueuedConnection);
    connect(threadForMac,SIGNAL(started()),this,SLOT(clicked()),Qt::QueuedConnection);
    connect(this,SIGNAL(setMac(QString)),Mac,SLOT(MacAdd(QString)),Qt::QueuedConnection);
    connect(Mac,SIGNAL(add(QString,QString,QString)),this,SLOT(SetMac1Mac2Ip(QString,QString,QString)),Qt::QueuedConnection);

    connect(threadForCom,SIGNAL(started()),Com,SLOT(ComFound()),Qt::QueuedConnection);
    connect(Com,SIGNAL(AddCom(QString,QString)),this,SLOT(findTEXT(QString,QString)),Qt::QueuedConnection);

    connect(Port,SIGNAL(Green_lamp_232()),this,SLOT(Green_lamp_232()),Qt::QueuedConnection);
    connect(Potok,SIGNAL(Green_lamp_232()),this,SLOT(Green_lamp_232()),Qt::QueuedConnection);
    connect(Potok,SIGNAL(Green_lamp_485()),this,SLOT(Green_lamp_485()),Qt::QueuedConnection);
    connect(Port,SIGNAL(Red_lamp_232()),this,SLOT(Red_lamp_232()),Qt::QueuedConnection);
    connect(Potok,SIGNAL(Red_lamp_232()),this,SLOT(Red_lamp_232()),Qt::QueuedConnection);
    connect(Potok,SIGNAL(Red_lamp_485()),this,SLOT(Red_lamp_485()),Qt::QueuedConnection);
    connect(this,SIGNAL(BtnClose()),this,SLOT(Btn_close()),Qt::QueuedConnection);
    connect(this,SIGNAL(BtnOpen()),this,SLOT(Btn_open()),Qt::QueuedConnection);
    connect(Port,SIGNAL(closeBtn()),this,SLOT(Btn_open()),Qt::QueuedConnection);
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(Refresh()),Qt::QueuedConnection);


    connect(ui->openHex,SIGNAL(clicked(bool)),this,SLOT(Refresh()),Qt::QueuedConnection);
    connect(ui->openHex,SIGNAL(clicked(bool)),this,SLOT(hex()),Qt::QueuedConnection);
    connect(Port,SIGNAL(on_open()),this,SLOT(on_open()),Qt::QueuedConnection);
    connect(Port,SIGNAL(off_open()),this,SLOT(off_open()),Qt::QueuedConnection);
    connect(Potok,SIGNAL(off_prg()),this,SLOT(off_prg()),Qt::QueuedConnection);
    connect(Potok,SIGNAL(on_prg()),this,SLOT(on_prg()),Qt::QueuedConnection);
    connect(Potok,SIGNAL(off_test()),this,SLOT(off_test()),Qt::QueuedConnection);
    connect(Potok,SIGNAL(on_test()),this,SLOT(on_test()),Qt::QueuedConnection);
    connect(Port,SIGNAL(off_test()),this,SLOT(off_test()),Qt::QueuedConnection);
    connect(Port,SIGNAL(on_test()),this,SLOT(on_test()),Qt::QueuedConnection);
    connect(Port,SIGNAL(close_port_intest()),Port,SLOT(closePort()),Qt::QueuedConnection);
    connect(Port,SIGNAL(ping()),Potok,SLOT(Ping()),Qt::QueuedConnection);
    connect(Port,SIGNAL(saveReport()),this,SLOT(SaveRep()),Qt::QueuedConnection);



    connect(this,SIGNAL(setDB(QString,QString,QString,QString)),f2,SLOT(setParam(QString,QString,QString,QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(rundb()),f2,SLOT(run()),Qt::QueuedConnection);
    connect(this, SIGNAL(rundb()),f2,SLOT(exec()));
    thread->start();
    portThread->start();
    threadForMac->start();
    threadForCom->start();
    FormTwo->start();

    QRegExp rx( "(   |[0-9]  | [0-9] |  [0-9]|[0-9][0-9] | [0-9][0-9]|[0-1][0-9][0-9]|2[0-4][0-9]|25[0-5])."
                                      "(   |[0-9]  | [0-9] |  [0-9]|[0-9][0-9] | [0-9][0-9]|[0-1][0-9][0-9]|2[0-4][0-9]|25[0-5])."
                                      "(   |[0-9]  | [0-9] |  [0-9]|[0-9][0-9] | [0-9][0-9]|[0-1][0-9][0-9]|2[0-4][0-9]|25[0-5])."
                                      "(   |[0-9]  | [0-9] |  [0-9]|[0-9][0-9] | [0-9][0-9]|[0-1][0-9][0-9]|2[0-4][0-9]|25[0-5])" ); // валидатор для айпи
        QRegExpValidator *validator = new QRegExpValidator(QRegExp(rx),ui->ip_line);


    ui->ip_line->setValidator(validator);
    ui->ip_line->setInputMask("000.000.000.000");



    QRegExp rx2("([0-9A-F]{2}[:-]){5}([0-9A-F]{2})"); //валидатор для мака
    QRegExp rx3("([0-9]{5})"); //валидатор для мака
    QRegExpValidator *validator2 = new QRegExpValidator(QRegExp(rx2),ui->Mac1_line);
    QRegExpValidator *validator3 = new QRegExpValidator(QRegExp(rx2),ui->Mac2_line);
    QRegExpValidator *validator4 = new QRegExpValidator(QRegExp(rx3),ui->numbCP);
    ui->numbCP->setValidator(validator4);
    ui->Mac1_line->setValidator(validator2);
    ui->Mac2_line->setValidator(validator3);
    ui->Mac1_line->setInputMask("hh:hh:hh:hh:hh:hh");
    ui->Mac2_line->setInputMask("hh:hh:hh:hh:hh:hh");




    ui->TypeCmbBox->addItem("ЦП");
    ui->TypeCmbBox->addItem("ЦПК");
    ui->TypeCmbBox->addItem("ЦПР");

    ui->ip_line->setDisabled(true);
    ui->Mac1_line->setDisabled(true);
    ui->Mac2_line->setDisabled(true);

    ui->baud->addItem(QLatin1String("1200"),QSerialPort::Baud1200);
    ui->baud->addItem(QLatin1String("2400"),QSerialPort::Baud2400);
    ui->baud->addItem(QLatin1String("4800"),QSerialPort::Baud4800);
    ui->baud->addItem(QLatin1String("9600"),QSerialPort::Baud9600);
    ui->baud->addItem(QLatin1String("19200"),QSerialPort::Baud19200);
    ui->baud->addItem(QLatin1String("38400"),QSerialPort::Baud38400);
    ui->baud->addItem(QLatin1String("57600"),QSerialPort::Baud57600);
    ui->baud->addItem(QLatin1String("115200"),QSerialPort::Baud115200);


    ui->patirity->addItem(QLatin1String("0"), QSerialPort::NoParity);
    ui->patirity->addItem(QLatin1String("2"), QSerialPort::EvenParity);
    ui->patirity->addItem(QLatin1String("3"), QSerialPort::OddParity);
    ui->patirity->addItem(QLatin1String("4"), QSerialPort::SpaceParity);
    ui->patirity->addItem(QLatin1String("5"), QSerialPort::MarkParity);




    QString line,fname = QDir::currentPath()+"/settings",DirForStart;
    QFile settings(fname);
    int w,h,idx;
    QList<int> list;
    if (QFile::exists(fname))
    {
    if(settings.open(QIODevice::ReadWrite))
    {

        //line = settings.read(4);

        line = settings.readLine();
        line = line.left(line.indexOf( "\r"));
        ui->portBox->addItem(line);





        line = settings.readLine();
        line = line.left(line.indexOf( "\r"));


        ui->portBox2->addItem(line);

        line = settings.read(1);



        ui->baud->setCurrentIndex(line.toInt());
        line = settings.read(1);

        ui->patirity->setCurrentIndex(line.toInt());

        line = settings.read(2);

        line = settings.readLine();

        w = line.toInt();

        line = settings.readLine();

        h = line.toInt();


        line = settings.readLine();
        list.push_back(line.toInt());

        line = settings.readLine();
        list.push_back(line.toInt());

        line = settings.readLine();

        int count1 = line.toInt();

        line = settings.readLine();
        idx = line.toInt();
        qDebug()<<count1;
        for (int i = 0; i<count1;i++)
        {
            line = settings.readLine();
            line.chop(2);
            ui->FamComBox->addItem(line);
            qDebug()<<line;
        }
        ui->FamComBox->setCurrentIndex(idx);

        LoginDB = settings.readLine();
        PassDB = settings.readLine();
        AdressDB = settings.readLine();
        NameDB = settings.readLine();

        line = settings.readLine();
        DirForStart=line;



        qDebug() <<line;

        settings.close();
    }
    }
    else
    {
        ui->consol->insertPlainText("Ошибка открытия файла настроек sittings");
        DirForStart = QDir::currentPath();

    }









    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //обновляем окна с ком портами
        {

        ui->consol->moveCursor(QTextCursor::End);
        ui->consol->insertPlainText(info.portName()+"  ");
        ui->consol->insertPlainText(info.description()+"\r");

        }


    model = new QDirModel(this); //модель для окна выбора файла

    QStringList filters;




    filters << "*.hex";

    model->setReadOnly(false);
    model->setNameFilters(filters);
    model->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);

    model->setFilter(QDir::Files|QDir::AllDirs|QDir::NoDotAndDotDot);


    ui->treeView->setModel(model);
    QModelIndex index;
    if (QFile::exists(DirForStart))
    index = model->index(DirForStart);
    else
         index = model->index(QDir::currentPath());




    ui->treeView->setRootIndex(model->index(""));
    ui->treeView->expand(index);
    ui->treeView->scrollTo(index);
    ui->treeView->setCurrentIndex(index);
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);



    ui->consol->moveCursor(QTextCursor::End); //помещение курсора вниз консоли
    ui->label_11->setStyleSheet("color: rgb(255, 35, 35)");
    ui->label_12->setStyleSheet("color: rgb(255, 35, 35)");



    QIcon winIcon(QDir::currentPath()+"/icon1.ico");
       this->setWindowIcon(winIcon);

    int hd = QApplication::desktop()->height();
    int wd = QApplication::desktop()->width();

    this->setGeometry(wd/2-w/2,hd/2-h/2, w,h);

    ui->consol->moveCursor(QTextCursor::End);

    QString str1;
    char buffer[256];
    DWORD size = 256;
    GetComputerNameA(buffer,&size);
    QByteArray asd = buffer;
    str1= "Имя ПК: ";
    ui->consol->insertPlainText(str1);
    Name_pk = asd.data();
    ui->consol->insertPlainText(Name_pk+"\r");
    ui->consol->insertPlainText("Дата: "+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:MM:ss")+"\r");
    ui->consol->setReadOnly(true);
    ui->lineFile->setReadOnly(true);

    ui->splitter->setSizes(list);

    QString tmp;
    int max= DirForStart.size();
    if (max>5)
    {
    tmp[3]=DirForStart[max-1];
    tmp[2]=DirForStart[max-2];
    tmp[1]=DirForStart[max-3];
    tmp[0]=DirForStart[max-4];
    }
    if (tmp == ".hex")
    {
        QString tempp=DirForStart,t2; int count;
        for(int i=tempp.size();i>1;i--)
            if (tempp[i]=='/')
            {
                count = i;
                break;
            }
        count=tempp.size()-count-1;
        for(int i = 0 ;i<count;i++)
            t2[i]=tempp[tempp.size()-count+i];
        ui->lineFile->setText(t2);

    }
    else ui->lineFile->setText("Выберите файл *.hex");

    LoginDB= LoginDB.left(LoginDB.indexOf( "\r"));
    PassDB= PassDB.left(PassDB.indexOf( "\r"));
    AdressDB= AdressDB.left(AdressDB.indexOf( "\r"));
    NameDB= NameDB.left(NameDB.indexOf( "\r"));


    ui->numbCP->setFocus();


}



MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::Refresh()
{


    QModelIndex index;
    QModelIndexList lst = ui->treeView->selectionModel()->selectedIndexes();
    QString DirForStart=model->filePath(lst.at(0));


    QStringList filters;




    filters << "*.hex";

    model->setReadOnly(false);
    model->setNameFilters(filters);
    model->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);

    model->setFilter(QDir::Files|QDir::AllDirs|QDir::NoDotAndDotDot);


    ui->treeView->setModel(model);

    if (QFile::exists(DirForStart))
        index = model->index(DirForStart);
    else
        index = model->index(QDir::currentPath());


    ui->treeView->setRootIndex(model->index(""));
    ui->treeView->expand(index);
    ui->treeView->scrollTo(index);
    ui->treeView->setCurrentIndex(index);
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QString tmp;
    int max= DirForStart.size();
    if (max>5)
    {
    tmp[3]=DirForStart[max-1];
    tmp[2]=DirForStart[max-2];
    tmp[1]=DirForStart[max-3];
    tmp[0]=DirForStart[max-4];
    }
    if (tmp == ".hex")
    {
        QString tempp=DirForStart,t2; int count;
        for(int i=tempp.size();i>1;i--)
            if (tempp[i]=='/')
            {
                count = i;
                break;
            }
        count=tempp.size()-count-1;
        for(int i = 0 ;i<count;i++)
            t2[i]=tempp[tempp.size()-count+i];
        ui->lineFile->setText(t2);

    }
    else ui->lineFile->setText("Выберите файл *.hex");



}

void MainWindow::Green_lamp_232()
{
    ui->label_12->setStyleSheet("color: rgb(41, 255, 41)");
    BtnClose();
}
void MainWindow::Green_lamp_485()
{
    ui->label_11->setStyleSheet("color: rgb(41, 255, 41)");
}
void MainWindow::Red_lamp_232()
{
    ui->label_12->setStyleSheet("color: rgb(255, 35, 35)");
    BtnOpen();
}
void MainWindow::Red_lamp_485()
{
    ui->label_11->setStyleSheet("color: rgb(255, 35, 35)");
}
void MainWindow::Btn_close()
{
    ui->portBtn->setText("Закрыть порт");
    flag = false;
}
void MainWindow::Btn_open()
{
    ui->portBtn->setText("Открыть порт");
    flag = true;
}
void MainWindow::on_portBtn_clicked()
{

    if (flag == true)
    {
        QString com1= ui->portBox->currentText();
        open232(com1,ui->baud->currentText().toInt(),ui->patirity->currentText().toInt());

        {

            ui->portBtn->setText("Закрыть порт");
            flag = false;
            return;
        }
    }
    if (flag == false)
    {
        close232();

        qDebug()<<"Открыть порт";
        ui->portBtn->setText("Открыть порт");
        flag = true;
        return;
    }
}

void MainWindow::findTEXT(QString com1, QString com2)
{

        QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
        textCharFormat.setForeground(Qt::black);
        ui->consol->setCurrentCharFormat(textCharFormat);



        if( (ui->portBox->findText(com1,0))==-1)
        {

            ui->portBox->addItem(com1);

            qDebug()<<com1;


        }

        if( (ui->portBox2->findText(com2,0))==-1)
        {
            qDebug()<<ui->portBox2->findText(com2,0);
            ui->portBox2->addItem(com2);



        }



}

void MainWindow::clicked()
{
    QModelIndexList lst = ui->treeView->selectionModel()->selectedIndexes();
    QString dir=model->filePath(lst.at(0)),tmp;

    int max= dir.size();
    if (max>5){
    tmp[3]=dir[max-1];
    tmp[2]=dir[max-2];
    tmp[1]=dir[max-3];
    tmp[0]=dir[max-4];

    if (tmp == ".hex")
        setMac(dir);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QString tmp,fname = QDir::currentPath()+"/settings";
    QByteArray line; int  t,w,h;


    QFile settings(fname);
    settings.open(QIODevice::WriteOnly);

    h = ui->centralWidget->height();
    w = ui->centralWidget->width();
    tmp = ui->portBox->currentText() + "\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,6);


    tmp = ui->portBox2->currentText() + "\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,6);


    t = ui->baud->currentIndex();

    line= QByteArray::number(t);

    settings.write(line,1);
    t = ui->patirity->currentIndex() ;
    line= QByteArray::number(t);

    settings.write(line,1);



    tmp = "\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,2);

    line = QByteArray::number(w);
    settings.write(line,line.size());

    tmp = "\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,2);

    line = QByteArray::number(h);
    settings.write(line,line.size());

    tmp = "\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,2);

    QList<int> list;
    int a,b;
    list = ui->splitter->sizes();
    a = list[0];
    b = list[1];
    line = QByteArray::number(a);

    settings.write(line,line.size());
    tmp = "\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,2);
    line = QByteArray::number(b);

    settings.write(line,line.size());
    tmp = "\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,2);
    line = QByteArray::number(ui->FamComBox->count());
    settings.write(line,line.size());
    tmp = "\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,2);
    t = ui->FamComBox->currentIndex();

    line= QByteArray::number(t);

    settings.write(line,1);
    tmp = "\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,2);
    for (int k = 0; k<ui->FamComBox->count();k++ )
       {
        line = ui->FamComBox->itemText(k).toUtf8();
        settings.write(line,line.size());
        tmp = "\r\n";
        line = tmp.toLocal8Bit();
        settings.write(line,2);

    }

    tmp = LoginDB+"\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,line.size());

    tmp = PassDB+"\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,line.size());


    tmp = AdressDB+"\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,line.size());

    tmp = NameDB+"\r\n";
    line = tmp.toLocal8Bit();
    settings.write(line,line.size());




    QModelIndexList lst = ui->treeView->selectionModel()->selectedIndexes();


    if(!lst.isEmpty())
    {
    line=model->filePath(lst.at(0)).toUtf8();
    settings.write(line,line.size());

    }
    event->accept();

}

void MainWindow::Print(QString str) //печать из потока
{
    ui->consol->moveCursor(QTextCursor::End);
    QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
    textCharFormat.setForeground(Qt::black);
    ui->consol->setCurrentCharFormat(textCharFormat);

    ui->consol->insertPlainText(str+"\r");


}
void MainWindow::Print1(QString str) //печать из потока
{
   ui->consol->moveCursor(QTextCursor::End);
   QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
   textCharFormat.setForeground(Qt::black);
   ui->consol->setCurrentCharFormat(textCharFormat);

   ui->consol->insertPlainText(str);



}
void MainWindow::Print2(QString str) //печать из потока
{
   ui->consol->moveCursor(QTextCursor::End);
   QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
   textCharFormat.setForeground(Qt::gray);
   ui->consol->setCurrentCharFormat(textCharFormat);

   ui->consol->insertPlainText(str);



}






void MainWindow::hex() // открыть файл, выбор файла для обработки
{   QFileDialog dialog;
    QString DirName = dialog.getOpenFileName(this, QString("Открыть Файл"), "", "*.hex");

    if(DirName!="")
    {
    QModelIndex index1 = model->index(DirName);


    ui->treeView->expand(index1);
    ui->treeView->scrollTo(index1);
    ui->treeView->setCurrentIndex(index1);
    ui->treeView->resizeColumnToContents(0);
    setMac(DirName);
    QString tmp;
    int max= DirName.size();
    if (max>5)
    {
    tmp[3]=DirName[max-1];
    tmp[2]=DirName[max-2];
    tmp[1]=DirName[max-3];
    tmp[0]=DirName[max-4];
    }
    if (tmp == ".hex")
    {
        QString tempp=DirName,t2; int count;
        for(int i=tempp.size();i>1;i--)
            if (tempp[i]=='/')
            {
                count = i;
                break;
            }
        count=tempp.size()-count-1;
        for(int i = 0 ;i<count;i++)
            t2[i]=tempp[tempp.size()-count+i];
        ui->lineFile->setText(t2);

    }

     }


}
void MainWindow::on_RecordBtn_clicked() //сохраняет лог файл
{

    QString fname = "log"+QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".txt";
    QFile file(fname);
    file.open(QIODevice::WriteOnly);
    file.write(ui->consol->toPlainText().toUtf8());
    file.close();
    ui->consol->moveCursor(QTextCursor::End);
    QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
    textCharFormat.setForeground(Qt::black);
    ui->consol->insertPlainText("Сообщение сохраненено \r");

}
void MainWindow::on_ClearBtn_clicked() //очистить лог
{
    ui->consol->clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //обновляем окна с ком портами
        {

        ui->consol->moveCursor(QTextCursor::End);
        ui->consol->insertPlainText(info.portName()+"  ");
        ui->consol->insertPlainText(info.description()+"\r");

        }
    QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
    textCharFormat.setForeground(Qt::black);
   QString str1;
    str1= "Имя ПК: ";
    ui->consol->insertPlainText(str1);
    str1 = Name_pk;
    ui->consol->insertPlainText(str1+"\r");


    ui->consol->insertPlainText("Дата: "+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:MM:ss")+"\r");

}



void MainWindow::on_pushButton_2_clicked() //кнопка Программирование. проверка всех галочек и прередача их в поток
{
    if(ui->EraseAll->checkState()==Qt::Checked)
        eraseAll =TRUE;
    else
        eraseAll =FALSE;

    if(ui->Erase->checkState()==Qt::Checked)
        erase =TRUE;
    else
        erase =FALSE;
    if(ui->verify->checkState()==Qt::Checked)
        verify =TRUE;
    else
        verify =FALSE;
    if(ui->Programm->checkState()==Qt::Checked)
        program =TRUE;
    else
        program =FALSE;

    QModelIndexList lst = ui->treeView->selectionModel()->selectedIndexes();
    QString dir=model->filePath(lst.at(0));

    QString com= ui->portBox->currentText();
    CheckParam(erase,eraseAll,program,verify);
    //
    QString temp=dir,temp2; int count;
    for(int i=temp.size();i>1;i--)
        if (temp[i]=='/')
        {
            count = i;
            break;
        }
    count=temp.size()-count-1;
    for(int i = 0 ;i<count;i++)
        temp2[i]=temp[temp.size()-count+i];
    QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
    textCharFormat.setForeground(Qt::black);
    ui->consol->setCurrentCharFormat(textCharFormat);
     ui->consol->insertPlainText("Выбран файл: " + temp2 + "\r");
     StartRun(com,dir);
}




void MainWindow::on_pushButton_5_clicked() //кнопка записи в файл маков и айпи
{
    QString ip,mac1,mac2;
    ip  = ui->ip_line->text();
    mac1 = ui->Mac1_line->text();
    mac2 = ui->Mac2_line->text();
    QModelIndexList lst = ui->treeView->selectionModel()->selectedIndexes();
    QString dir=model->filePath(lst.at(0));
    GetIpMac(ip,mac1,mac2,dir);
}

void MainWindow::AddTooBar(int i) //ход прогресс бара
{
     ui->progressBar->setValue(i);
}
void MainWindow::setMaxPrBar(int max) //переполнение прогресс бара и установка макс значения
{
    if( ui->progressBar->text() == "100%")
        ui->progressBar->setValue(0);
   ui->progressBar->setMaximum(max-1);

}

void MainWindow::setRrBarNull() //переполнение прогресс бара и установка макс значения
{

     ui->progressBar->setValue(0);


}
void MainWindow::on_testBtn_clicked()
{
    QString com1= ui->portBox->currentText();
    QString com2= ui->portBox2->currentText();
    TestOn(com1,com2);

}

void MainWindow::on_checkBox_clicked()
{
    if (flag2 ==true)
    {
        ui->ip_line->setEnabled(true);
        ui->Mac1_line->setEnabled(true);
        ui->Mac2_line->setEnabled(true);
        flag2 = false;
        return;
    }
    if (flag2 ==false)
    {
        ui->ip_line->setDisabled(true);
        ui->Mac1_line->setDisabled(true);
        ui->Mac2_line->setDisabled(true);
        flag2 = true;
        return;
    }
}
void MainWindow::SetMac1Mac2Ip(QString mac1,QString mac2,QString ip)
{
    ui->Mac1_line->setText(mac1);
    ui->Mac2_line->setText(mac2);
    ui->ip_line->setText(ip);
}
void MainWindow::off_prg()
{
    ui->Erase->setEnabled(true);
    ui->EraseAll->setEnabled(true);
    ui->Programm->setEnabled(true);
    ui->verify->setEnabled(true);
    ui->portBox->setEnabled(true);
    ui->portBtn->setEnabled(true);
    ui->patirity->setEnabled(true);
    ui->baud->setEnabled(true);
}
void MainWindow::on_prg()
{
    ui->Erase->setDisabled(true);
    ui->EraseAll->setDisabled(true);
    ui->Programm->setDisabled(true);
    ui->verify->setDisabled(true);
    ui->portBox->setDisabled(true);
    ui->portBtn->setDisabled(true);
    ui->patirity->setDisabled(true);
    ui->baud->setDisabled(true);
}
void MainWindow::off_test()
{

    ui->portBox2->setEnabled(true);
}
void MainWindow::on_test()
{

    ui->portBox2->setDisabled(true);
}
void MainWindow::off_open()
{
    ui->Erase->setEnabled(true);
    ui->EraseAll->setEnabled(true);
    ui->Programm->setEnabled(true);
    ui->verify->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->baud->setEnabled(true);
    ui->patirity->setEnabled(true);
    ui->portBox->setEnabled(true);
}
void MainWindow::on_open()
{
    ui->Erase->setDisabled(true);
    ui->EraseAll->setDisabled(true);
    ui->Programm->setDisabled(true);
    ui->verify->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
    ui->baud->setDisabled(true);
    ui->patirity->setDisabled(true);
    ui->portBox->setDisabled(true);
}

void MainWindow::on_getMacPromBD_clicked()
{
    QString type,numb,temp,temp2,temp3,mac1;
    int number;


    QModelIndexList lst = ui->treeView->selectionModel()->selectedIndexes();
    QString dir=model->filePath(lst.at(0));
    QString tempp=dir,t2; int count;
    for(int i=tempp.size();i>1;i--)
        if (tempp[i]=='/')
        {
            count = i;
            break;
        }
    count=tempp.size()-count-1;
    for(int i = 0 ;i<count;i++)
        t2[i]=tempp[tempp.size()-count+i];



    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(AdressDB);
    db.setDatabaseName(NameDB);
    db.setUserName(LoginDB);
    db.setPassword(PassDB);
    if (db.open())
    {
        ui->consol->moveCursor(QTextCursor::End);
        QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
        textCharFormat.setForeground(Qt::black);
        ui->consol->insertPlainText("База данных открыта успешно \r");
    }
    else
    {
         ui->consol->moveCursor(QTextCursor::End);
        QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
        textCharFormat.setForeground(Qt::black);
        ui->consol->insertPlainText("Ошибка открытия Базы данных \r");
        return;

    }

    if (ui->TypeCmbBox->currentText()=="ЦП")
        type="ЦП";
    else if (ui->TypeCmbBox->currentText()=="ЦПК")
            type="ЦПК";
    else if (ui->TypeCmbBox->currentText()=="ЦПР")
            type="ЦПР";


    numb = ui->numbCP->text();
    qDebug()<<numb;
    if (numb =="")
    {
        ui->consol->moveCursor(QTextCursor::End);
        QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
        textCharFormat.setForeground(Qt::black);
        ui->consol->insertPlainText("Не введен номер ЦП/ЦПК/ЦПР \r");
        db.close();

        return;
    }

    QSqlQuery query(db);
    bool a;
      if (type=="ЦПК")   // 1 MAC
      {

          {


      temp = "SELECT MAC1 FROM table1 WHERE id=" +numb+" AND Тип='"+type  + "'";

      qDebug()<<temp;
      query.exec(temp);
      query.next();
      qDebug()<<query.value(0).toString();


      if (query.value(0).toString()!="")
      {




          QMessageBox msBox;
          msBox.setText("Запрашиваемое утройство: " + ui->TypeCmbBox->currentText() + " с номером "  + numb + " \r\nВ БД уже записано!\r\nMAC1:"+query.value(0).toString()+" \r\nПолучить MAC из БД и записать в файл "+t2+" ?");
          msBox.setIcon(QMessageBox::Question);
          QPushButton *ok = msBox.addButton(tr("Ok"),QMessageBox::ActionRole);
          msBox.addButton(QObject::tr("Отмена"), QMessageBox::ActionRole);
          msBox.exec();
          if (msBox.clickedButton()== ok)
          {

              ui->consol->moveCursor(QTextCursor::End);
              QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
              textCharFormat.setForeground(Qt::black);
              ui->consol->insertPlainText("Получено из БД: " + ui->TypeCmbBox->currentText() + " с номером "  + numb + " имеет MAC Адрес \rMAC1:  "+query.value(0).toString() + " \r");


              if ((t2[0]!='k')&&(t2[1]!='e')&&(t2[2]!='r')&&(t2[3]!='n')&&(t2[4]!='e')&&(t2[5]!='l'))
              {   ui->consol->insertPlainText("Нельзя записать в файл: "+t2+"\rВыбирите файл: kernel \r");

              }
              else
              {
                  GetIpMac("10.20.10.20",query.value(0).toString(),"00:00:00:00:00:00",dir);
                  ui->consol->moveCursor(QTextCursor::End);
                  QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
                  textCharFormat.setForeground(Qt::black);
                  ui->consol->insertPlainText("MAC Адрес удачно записан в файл: "+ t2+"\r");
              }
              ui->Mac1_line->setText(query.value(0).toString());
              ui->Mac2_line->setText("00:00:00:00:00:00");
              ui->ip_line->setText("10.20.10.20");



          }
          return;
      }
      else
      {
         temp = "SELECT GREATEST(max(MAC1), max(MAC2)) FROM table1;";
         query.exec(temp);

         query.next();
         qDebug()<<query.value(0).toString();
         temp2=query.value(0).toString();
         if (temp2[16]!='F')
            {
              temp3[0]=temp2[16];
              temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
              temp2[16]=temp3[0];
            }
         else
         {
             temp2[16]='0';
             if (temp2[15]!='F')
             {
                 temp3[0]=temp2[15];
                 temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                 temp2[15]=temp3[0];
             }
             else
                 {
                     temp2[15]='0';
                     if (temp2[13]!='F')
                     {
                         temp3[0]=temp2[13];
                         temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                         temp2[13]=temp3[0];
                     }
                     else
                         {
                            temp2[13]='0';
                             if (temp2[12]!='F')
                            {
                                 temp3[0]=temp2[12];
                                 temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                 temp2[12]=temp3[0];
                            }
                                else
                                {
                                    temp2[12]='0';
                                    if (temp2[10]!='F')
                                    {
                                        temp3[0]=temp2[10];
                                        temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                        temp2[10]=temp3[0];
                                 }
                                         else
                                        {
                                            temp2[10]='0';
                                            if (temp2[9]!='F')
                                            {
                                                temp3[0]=temp2[9];
                                                temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                temp2[9]=temp3[0];
                                             }
                                                 else
                                                {
                                                        temp2[9]='0';
                                                        if (temp2[7]!='F')
                                                        {
                                                            temp3[0]=temp2[7];
                                                            temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                            temp2[7]=temp3[0];
                                                        }
                                                        else
                                                        {
                                                                temp2[7]='0';
                                                                if (temp2[6]!='F')
                                                                {
                                                                    temp3[0]=temp2[6];
                                                                    temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                    temp2[6]=temp3[0];
                                                                }
                                                                else
                                                                {
                                                                        temp2[6]='0';
                                                                        if (temp2[4]!='F')
                                                                        {
                                                                            temp3[0]=temp2[4];
                                                                            temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                            temp2[4]=temp3[0];
                                                                        }
                                                                        else
                                                                        {
                                                                                temp2[4]='0';
                                                                                if (temp2[3]!='F')
                                                                                {
                                                                                    temp3[0]=temp2[3];
                                                                                    temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                                    temp2[3]=temp3[0];
                                                                                }
                                                                                else
                                                                                {
                                                                                        temp2[3]='0';
                                                                                        if (temp2[1]!='F')
                                                                                        {
                                                                                            temp3[0]=temp2[1];
                                                                                            temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                                            temp2[1]=temp3[0];
                                                                                        }
                                                                                        else
                                                                                        {
                                                                                            temp2[1]='0';
                                                                                            temp3[0]=temp2[0];
                                                                                            temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                                            temp2[0]=temp3[0];
                                                                                        }


                                                                                }

                                                                        }

                                                                }

                                                        }

                                                }

                                         }


                                 }

                         }
                 }
         }

         temp = "SELECT max(№) FROM table1";
         query.exec(temp);

         query.next();


         number = query.value(0).toInt()+1;
         qDebug()<<number;


         QMessageBox msBox;
         msBox.setText("ВНИМАНИЕ! \r\nОтсутсвует в БД: " + ui->TypeCmbBox->currentText() + " с номером "  + numb + ". \r\n Добавить в БД и записать в файл "+t2+" ?");
         msBox.setIcon(QMessageBox::Question);
         QPushButton *ok = msBox.addButton(tr("Ok"),QMessageBox::ActionRole);
         msBox.addButton(QObject::tr("Отмена"), QMessageBox::ActionRole);

         msBox.exec();
         if (msBox.clickedButton()== ok)
         {
             ui->consol->moveCursor(QTextCursor::End);
             QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
             textCharFormat.setForeground(Qt::black);
             ui->consol->insertPlainText("Добавлено в БД: " + ui->TypeCmbBox->currentText() + " с номером "  + numb + " MAC Адрес \rMAC1: "+temp2 + " \r");
             if ((t2[0]!='k')&&(t2[1]!='e')&&(t2[2]!='r')&&(t2[3]!='n')&&(t2[4]!='e')&&(t2[5]!='l'))
             {   ui->consol->insertPlainText("Нельзя записать в файл: "+t2+"\rВыбирите файл: kernel \r");

             }
             else
             {
                 ui->consol->moveCursor(QTextCursor::End);
                 QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
                 textCharFormat.setForeground(Qt::black);
                 ui->consol->insertPlainText("MAC Адрес удачно записан в файл: "+ t2+"\r");
                 GetIpMac("10.20.10.20",temp2,"00:00:00:00:00:00",dir);

             }
             ui->Mac1_line->setText(temp2);
             ui->Mac2_line->setText("00:00:00:00:00:00");
             ui->ip_line->setText("10.20.10.20");

             QString str_ins = "INSERT INTO table1 (№, ID, Тип, MAC1, Дата, Проверяющий, ПК) "
                     "VALUES (%1, %2, '%3', '%4', '%5','%6','%7');";
             temp = str_ins.arg(number)
                     .arg(numb)
                     .arg(type)
                     .arg(temp2)
                     .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:MM:ss"))
                     .arg(ui->FamComBox->currentText())
                     .arg(Name_pk);
             qDebug()<<temp;
             query.exec(temp);



         db.close();
         return;
       }

}
       }
       }
       QString m1,m2;
       if ((type=="ЦП")||(type=="ЦПР"))
       {



           temp = "SELECT MAC1 FROM table1 WHERE id=" +numb+" AND Тип='"+type  + "'";

           qDebug()<<temp;
           query.exec(temp);


           query.next();
           qDebug()<<query.value(0).toString();
           if (query.value(0).toString()!="")
           {
               m1=query.value(0).toString();
                   temp = "SELECT MAC2 FROM table1 WHERE id=" +numb+" AND Тип='"+type  + "'";
               query.exec(temp);
               query.next();
               ui->consol->moveCursor(QTextCursor::End);
               ui->consol->insertPlainText(query.value(0).toString() + "\r" );
               m2=query.value(0).toString();



               QMessageBox msBox;
               msBox.setText("Запрашиваемое утройство: " + ui->TypeCmbBox->currentText() + " с номером "  + numb + " \r\nВ БД уже записано!\r\nMAC1: "+m1+"\r\nMAC2: "+m2+" \r\nПолучить MAC из БД и записать в файл "+t2+" ?");
               msBox.setIcon(QMessageBox::Question);
               QPushButton *ok = msBox.addButton(tr("Ok"),QMessageBox::ActionRole);
               msBox.addButton(QObject::tr("Отмена"), QMessageBox::ActionRole);
               //msBox.setDefaultButton(Output);
               msBox.exec();
               if (msBox.clickedButton()== ok)
               {
                   ui->consol->moveCursor(QTextCursor::End);
                   ui->consol->insertPlainText("Получено из БД: " + ui->TypeCmbBox->currentText() + " с номером "  + numb + " имеет MAC Адрес \rMAC1: "+m1 +"\r\nMAC2: "+m2+"\r");

                   if ((t2[0]!='k')&&(t2[1]!='e')&&(t2[2]!='r')&&(t2[3]!='n')&&(t2[4]!='e')&&(t2[5]!='l'))
                   {   ui->consol->insertPlainText("Нельзя записать в файл: "+t2+"\rВыбирите файл: kernel \r");

                   }
                   else
                   {
                       GetIpMac("10.20.10.20",m1,m2,dir);
                       ui->consol->moveCursor(QTextCursor::End);
                       QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
                       textCharFormat.setForeground(Qt::black);
                       ui->consol->insertPlainText("MAC Адреса удачно записаны в файл: "+ t2+"\r");
                   }
                   ui->ip_line->setText("10.20.10.20");
                   ui->Mac1_line->setText(m1);
                   ui->Mac2_line->setText(m2);



               return;
           }
           }
           else
           {
              temp = "SELECT GREATEST(max(MAC1), max(MAC2)) FROM table1;";
              query.exec(temp);

              query.next();
              qDebug()<<query.value(0).toString();
              temp2=query.value(0).toString();
              if (temp2[16]!='F')
                 {
                   temp3[0]=temp2[16];
                   temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                   temp2[16]=temp3[0];
                 }
              else
              {
                  temp2[16]='0';
                  if (temp2[15]!='F')
                  {
                      temp3[0]=temp2[15];
                      temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                      temp2[15]=temp3[0];
                  }
                  else
                      {
                          temp2[15]='0';
                          if (temp2[13]!='F')
                          {
                              temp3[0]=temp2[13];
                              temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                              temp2[13]=temp3[0];
                          }
                          else
                              {
                                 temp2[13]='0';
                                  if (temp2[12]!='F')
                                 {
                                      temp3[0]=temp2[12];
                                      temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                      temp2[12]=temp3[0];
                                 }
                                     else
                                     {
                                         temp2[12]='0';
                                         if (temp2[10]!='F')
                                         {
                                             temp3[0]=temp2[10];
                                             temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                             temp2[10]=temp3[0];
                                      }
                                              else
                                             {
                                                 temp2[10]='0';
                                                 if (temp2[9]!='F')
                                                 {
                                                     temp3[0]=temp2[9];
                                                     temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                     temp2[9]=temp3[0];
                                                  }
                                                      else
                                                     {
                                                             temp2[9]='0';
                                                             if (temp2[7]!='F')
                                                             {
                                                                 temp3[0]=temp2[7];
                                                                 temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                 temp2[7]=temp3[0];
                                                             }
                                                             else
                                                             {
                                                                     temp2[7]='0';
                                                                     if (temp2[6]!='F')
                                                                     {
                                                                         temp3[0]=temp2[6];
                                                                         temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                         temp2[6]=temp3[0];
                                                                     }
                                                                     else
                                                                     {
                                                                             temp2[6]='0';
                                                                             if (temp2[4]!='F')
                                                                             {
                                                                                 temp3[0]=temp2[4];
                                                                                 temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                                 temp2[4]=temp3[0];
                                                                             }
                                                                             else
                                                                             {
                                                                                     temp2[4]='0';
                                                                                     if (temp2[3]!='F')
                                                                                     {
                                                                                         temp3[0]=temp2[3];
                                                                                         temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                                         temp2[3]=temp3[0];
                                                                                     }
                                                                                     else
                                                                                     {
                                                                                             temp2[3]='0';
                                                                                             if (temp2[1]!='F')
                                                                                             {
                                                                                                 temp3[0]=temp2[1];
                                                                                                 temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                                                 temp2[1]=temp3[0];
                                                                                             }
                                                                                             else
                                                                                             {
                                                                                                 temp2[1]='0';
                                                                                                 temp3[0]=temp2[0];
                                                                                                 temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                                                 temp2[0]=temp3[0];
                                                                                             }


                                                                                     }

                                                                             }

                                                                     }

                                                             }

                                                     }

                                              }


                                      }

                              }
                      }


            }

              mac1=temp2;



              if (temp2[16]!='F')
                 {
                   temp3[0]=temp2[16];
                   temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                   temp2[16]=temp3[0];
                 }
              else
              {
                  temp2[16]='0';
                  if (temp2[15]!='F')
                  {
                      temp3[0]=temp2[15];
                      temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                      temp2[15]=temp3[0];
                  }
                  else
                      {
                          temp2[15]='0';
                          if (temp2[13]!='F')
                          {
                              temp3[0]=temp2[13];
                              temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                              temp2[13]=temp3[0];
                          }
                          else
                              {
                                 temp2[13]='0';
                                  if (temp2[12]!='F')
                                 {
                                      temp3[0]=temp2[12];
                                      temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                      temp2[12]=temp3[0];
                                 }
                                     else
                                     {
                                         temp2[12]='0';
                                         if (temp2[10]!='F')
                                         {
                                             temp3[0]=temp2[10];
                                             temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                             temp2[10]=temp3[0];
                                      }
                                              else
                                             {
                                                 temp2[10]='0';
                                                 if (temp2[9]!='F')
                                                 {
                                                     temp3[0]=temp2[9];
                                                     temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                     temp2[9]=temp3[0];
                                                  }
                                                      else
                                                     {
                                                             temp2[9]='0';
                                                             if (temp2[7]!='F')
                                                             {
                                                                 temp3[0]=temp2[7];
                                                                 temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                 temp2[7]=temp3[0];
                                                             }
                                                             else
                                                             {
                                                                     temp2[7]='0';
                                                                     if (temp2[6]!='F')
                                                                     {
                                                                         temp3[0]=temp2[6];
                                                                         temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                         temp2[6]=temp3[0];
                                                                     }
                                                                     else
                                                                     {
                                                                             temp2[6]='0';
                                                                             if (temp2[4]!='F')
                                                                             {
                                                                                 temp3[0]=temp2[4];
                                                                                 temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                                 temp2[4]=temp3[0];
                                                                             }
                                                                             else
                                                                             {
                                                                                     temp2[4]='0';
                                                                                     if (temp2[3]!='F')
                                                                                     {
                                                                                         temp3[0]=temp2[3];
                                                                                         temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                                         temp2[3]=temp3[0];
                                                                                     }
                                                                                     else
                                                                                     {
                                                                                             temp2[3]='0';
                                                                                             if (temp2[1]!='F')
                                                                                             {
                                                                                                 temp3[0]=temp2[1];
                                                                                                 temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                                                 temp2[1]=temp3[0];
                                                                                             }
                                                                                             else
                                                                                             {
                                                                                                 temp2[1]='0';
                                                                                                 temp3[0]=temp2[0];
                                                                                                 temp3 = QString::number((temp3.toInt(&a,16)+1),16).toUpper();
                                                                                                 temp2[0]=temp3[0];
                                                                                             }


                                                                                     }

                                                                             }

                                                                     }

                                                             }

                                                     }

                                              }


                                      }

                              }
                      }


            }

              temp = "SELECT max(№) FROM table1";
              query.exec(temp);
              query.next();

              number = query.value(0).toInt()+1;
              qDebug()<<number;



              QMessageBox msBox;
              msBox.setText("ВНИМАНИЕ! \r\nОтсутсвует в БД: " + ui->TypeCmbBox->currentText() + " с номером "  + numb + ". \r\n Добавить в БД и записать в файл "+t2+" ?");
              msBox.setIcon(QMessageBox::Question);
              QPushButton *ok = msBox.addButton(tr("Ok"),QMessageBox::ActionRole);
              msBox.addButton(QObject::tr("Отмена"), QMessageBox::ActionRole);
              //msBox.setDefaultButton(Output);
              msBox.exec();
              if (msBox.clickedButton()== ok)
              {QString str_ins = "INSERT INTO table1 (№, ID, Тип, MAC1, MAC2, Дата, Проверяющий, ПК)"
                                 "VALUES (%1, %2, '%3', '%4','%5', '%6','%7','%8');";
                         temp = str_ins.arg(number)
                                 .arg(numb)
                                 .arg(type)
                                 .arg(mac1)
                                 .arg(temp2)
                                 .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:MM:ss"))
                                 .arg(ui->FamComBox->currentText())
                                 .arg(Name_pk);
                         qDebug()<<temp;
                         query.exec(temp);

                     ui->consol->moveCursor(QTextCursor::End);
                     QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
                     textCharFormat.setForeground(Qt::black);
                     ui->consol->insertPlainText("Добавлено в БД: " + ui->TypeCmbBox->currentText() + " с номером "  + numb + " MAC Адрес \rMAC1: "+mac1 + " \rMAC2: " + temp2 + "\r");

                  if ((t2[0]!='k')&&(t2[1]!='e')&&(t2[2]!='r')&&(t2[3]!='n')&&(t2[4]!='e')&&(t2[5]!='l'))
                  {   ui->consol->insertPlainText("Нельзя записать в файл: "+t2+"\rВыбирите файл: kernel \r");

                  }
                  else
                  {
                      GetIpMac("10.20.10.20",mac1,temp2,dir);
                      ui->consol->moveCursor(QTextCursor::End);
                      QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
                      textCharFormat.setForeground(Qt::black);
                      ui->consol->insertPlainText("MAC Адреса удачно записаны в файл: "+ t2+"\r");
                  }
                      ui->ip_line->setText("10.20.10.20");
                      ui->Mac1_line->setText(mac1);
                      ui->Mac2_line->setText(temp2);





              db.close();
              return;

           }
           }

       }


}



void MainWindow::on_pushButton_clicked()
{
    QString type,numb,temp;
    int number;


    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(AdressDB);
    db.setDatabaseName(NameDB);
    db.setUserName(LoginDB);
    db.setPassword(PassDB);
    if (db.open())
    {

        ui->consol->moveCursor(QTextCursor::End);
        QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
        textCharFormat.setForeground(Qt::black);
        ui->consol->insertPlainText("База данных открыта успешно \r");
    }
    else
    {

        ui->consol->moveCursor(QTextCursor::End);
        QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
        textCharFormat.setForeground(Qt::black);
        ui->consol->insertPlainText("Ошибка открытия Базы данных \r");
        return;

    }

    if (ui->TypeCmbBox->currentText()=="ЦП")
        type="ЦП";
    else if (ui->TypeCmbBox->currentText()=="ЦПК")
            type="ЦПК";
    else if (ui->TypeCmbBox->currentText()=="ЦПР")
            type="ЦПР";


    numb = ui->numbCP->text();
    qDebug()<<numb;
    if (numb =="")
    {
        ui->consol->moveCursor(QTextCursor::End);
        QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
        textCharFormat.setForeground(Qt::black);
        ui->consol->insertPlainText("Не введен номер ЦП/ЦПК/ЦПР \r");
        db.close();

        return;
    }
    QSqlQuery query(db);
    temp = "SELECT max(№) FROM table2";
    query.exec(temp);

    query.next();


    number = query.value(0).toInt()+1;
    qDebug()<<number;
    int report;
    QMessageBox msBox;
              msBox.setText("Выбирите тип отчета:");
              msBox.setIcon(QMessageBox::Question);
              QPushButton *one = msBox.addButton(tr("1 - До лакировки"),QMessageBox::ActionRole);
              QPushButton *two = msBox.addButton(tr("2 - После лакировки"),QMessageBox::ActionRole);
              QPushButton *cansel = msBox.addButton(tr("Отмена"),QMessageBox::ActionRole);

              one->setFocusPolicy(Qt::NoFocus);
              two->setFocusPolicy(Qt::NoFocus);

              msBox.exec();
              if (msBox.clickedButton()== one)
              {
              report = 1;
              }
              if (msBox.clickedButton()== two)
              {
              report = 2;

              }
              if (msBox.clickedButton()== cansel)

              {
                  return;
              }





    query.prepare("INSERT INTO table2 (№, ID, Тип, Номер_отчета, Дата, Проверяющий, ПК, Отчет) VALUES (:1, :2, :3, :4, :5, :6, :7, :8);");
    query.bindValue(":1",number);
    query.bindValue(":2",numb);
    query.bindValue(":3",type);
    query.bindValue(":4",report);
    query.bindValue(":5",QDateTime::currentDateTime().toString("yyyy-MM-dd HH:MM:ss"));
    query.bindValue(":6",ui->FamComBox->currentText());
    query.bindValue(":7",Name_pk);
    query.bindValue(":8", ui->consol->toPlainText());

    if (query.exec())
       {
        ui->consol->moveCursor(QTextCursor::End);
        QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
        textCharFormat.setForeground(Qt::black);
        ui->consol->insertPlainText("Отчет сохранен в базу данных \r");
        db.close();
        on_ClearBtn_clicked();
       }
   else {
        ui->consol->moveCursor(QTextCursor::End);
        QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
        textCharFormat.setForeground(Qt::black);
            ui->consol->insertPlainText("Ошибка сохранинения отчета в базу данных \r");
            db.close();
        }   return;
}

void  MainWindow::SaveRep()
{
    QMessageBox msBox;
    msBox.setText("Сохранить отчет в базу данных? ");
    msBox.setIcon(QMessageBox::Question);
    QPushButton *ok = msBox.addButton(tr("Ok"),QMessageBox::ActionRole);
    msBox.addButton(QObject::tr("Отмена"), QMessageBox::ActionRole);

    msBox.exec();
    if (msBox.clickedButton()== ok)
    {
        on_pushButton_clicked();
    }
}

void MainWindow::on_openReportsBtn_clicked()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(AdressDB);
    db.setDatabaseName(NameDB);
    db.setUserName(LoginDB);
    db.setPassword(PassDB);

    if (db.open())
    {
        ui->consol->moveCursor(QTextCursor::End);
        QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
        textCharFormat.setForeground(Qt::black);
        ui->consol->insertPlainText("База данных открыта успешно \r");
        db.close();
        Sleep(100);
        setDB(LoginDB,PassDB,AdressDB,NameDB);
        Sleep(100);
        rundb();
    }
    else
    {
         ui->consol->moveCursor(QTextCursor::End);
        QTextCharFormat textCharFormat = ui->consol->currentCharFormat();
        textCharFormat.setForeground(Qt::black);
        ui->consol->insertPlainText("Ошибка открытия Базы данных \r");
        return;

    }



}
// //////////////////////////////////////////////////////////////////////////////    сверху поток формы
// //////////////////////////////////////////////////////////////////////////////    снизу поток действий
// //////////////////////////////////////////////////////////////////////////////




void PotokThread::Start(QString portName, QString dir) //кнопка выполнить
{   QSerialPort com;
    QByteArray Rbuf,Wbuf;
    bool f;
    const char id_str[13]= "K4000_BOOTCP";
    //const char id_str[13]= "1986BOOTUART";
    com.setPortName(portName);

    ClosePort232();
    on_prg();
    //Sleep(500);
    SetMaxBar(6);
    str = "Программирование с COM портом "+ portName;
    PrintMess(str);
    if ((eraseAll==1)||(erase==1)||(program == 1)||(verify == 1))
    {
       str = "Выбранные параметры:";
       PrintMess(str);
    }
    else
    {
        str = "Программирование без параметров";
        PrintMess(str);
    }
    if (eraseAll==1)
    {
        str = "Очистить все";
        PrintMess(str);
    }
    if (erase==1)
    {
        str = "Очистить";
        PrintMess(str);
    }
     if (program == 1)
    {
        str = "Программировать";
        PrintMess(str);
    }
    if (verify == 1)
    {
        str = "Проверка";
        PrintMess(str);
    }

    if ((program == 1)||(verify == 1 ))
    {
        if(!GetHexCod(dir))
                {
                    str = "Ошибка открытия HEX-файла!";
                    PrintMess(str);
                    com.close();
                    Red_lamp_232();
                    off_prg();
                    setRrBarNull();
                    return;
                }

    }


    if  (!GetBootCod())
    {
        str="Ошибка открытия файла K4000_BOOTCP.HEX";
        PrintMess(str);
        com.close();
        Red_lamp_232();
        off_prg();
        setRrBarNull();
        return;
    }
        qDebug() << GetBootCod();




    com.setPortName(portName);                       //параметры ком порта 232
    com.setBaudRate(QSerialPort::Baud9600);
    com.setDataBits(QSerialPort::Data8);
    com.setParity(QSerialPort::NoParity);
    com.setStopBits(QSerialPort::OneStop);
    com.setFlowControl(QSerialPort::NoFlowControl);
    com.open(QIODevice::ReadWrite);
    if (com.isOpen())
        {
        str= "Порт открыт "+portName;
        PrintMess(str);
        str= "Скорость 9600";
        PrintMess(str);
        str= "Четнoсть 0";
        PrintMess(str);

        Green_lamp_232();
        }
    else
    {
        str="Ошибка открытия COM порта "+portName;
        PrintMess(str);
        com.close();
        Red_lamp_232();
        off_prg();
        setRrBarNull();
        return;
    }

    Rbuf.clear();
    Wbuf.clear();
    AddtoBar(1);
    str="Синхронизация... ";
    PrintMess1(str);
    Wbuf[0] = 0x0;
     com.waitForReadyRead(1000);
    Rbuf = com.readAll(); // очистка порта от мусора
    Rbuf.clear();

    for (int i = 0 ; i<512 ; i++)
    {
        com.write(Wbuf,1);
        com.flush();
    }
     Sleep(200);
    com.waitForReadyRead(500);
    com.waitForReadyRead(500);
    com.waitForReadyRead(500);
    com.waitForReadyRead(500);
    com.waitForReadyRead(500);
    com.waitForReadyRead(500);
    Rbuf = com.readAll();

    qDebug() << "Resive!";
    qDebug() << Rbuf;
    qDebug() << Rbuf.size();
    qDebug() << Rbuf.toHex().toInt();

    if (Rbuf.size() <3)
    {

            str="\rОшибка синхронизации ";
            PrintMess(str);
            com.close();
            Red_lamp_232();
            off_prg();
            setRrBarNull();
            return;

    }


    str =" OK";
    PrintMess(str);
    Wbuf.clear();
    Rbuf.clear();
    AddtoBar(2);
    qDebug() << Rbuf;

    Wbuf[0] = 'B';
    Wbuf[1] = 0x0;
    Wbuf[2] = (char)0xc2;
    Wbuf[3] = 0x01;
    Wbuf[4] = 0x0;
    com.write(Wbuf,5);
    com.flush();


    for (int t = 0; t<10;t++)
    {
    com.waitForReadyRead(100);
    Rbuf = com.readAll();
    if (Rbuf.size() > 0)
    qDebug() << "YES YES YES";
    qDebug() << Rbuf;
    }
   // Sleep(500);
    com.close();
    Sleep(200);
    com.setBaudRate(QSerialPort::Baud115200);   //смена скорости порта после синхронизации с к4000
    com.open(QIODevice::ReadWrite);



    com.write(Wbuf,5);
    com.flush();
    //Sleep(500);
    qDebug() << "SEND!";
    qDebug() << Wbuf;
    qDebug() << Wbuf.toHex();
    com.waitForReadyRead(400);

    for (int t = 0; t<10;t++)
    {
    com.waitForReadyRead(100);
    Rbuf = com.readAll();
    if (Rbuf.size() > 0)
    qDebug() << "YES YES YES";
    qDebug() << Rbuf;
    }
    Rbuf.clear();
    Wbuf.clear();
    Wbuf[0] =0xd;
    com.write(Wbuf,1);
    com.waitForReadyRead(100);
    Rbuf = com.readAll();
    qDebug() << Rbuf;
    qDebug() << Rbuf.size();
    if ((Rbuf.size()!=3)||(Rbuf[0]!=0xd)||(Rbuf[1]!=0xa)||(Rbuf[2]!=0x3e))
    {
        str = "Ошибка обмена ";
        PrintMess(str);
        com.close();
        Red_lamp_232();
        off_prg();
        setRrBarNull();
        return;
    }
    AddtoBar(3);
    str = "Установлена скорость COM порта 115200 ";
    PrintMess(str);
    str = "Загрузка загрузчика ";                      // заливка загрузчика
    PrintMess1(str);


    Wbuf[0] = 'L';
    Wbuf[1] = dwadrboot & 0xff;
    Wbuf[2] = (dwadrboot>>8) & 0xff;
    Wbuf[3] = 0;
    Wbuf[4] = 0x20;
    Wbuf[5] = ilboot & 0xff;
    Wbuf[6] = (ilboot>>8) & 0xff;
    Wbuf[7] = 0;
    Wbuf[8] = 0;
    com.write(Wbuf,9);
    com.flush();

    com.waitForReadyRead(200);
    Rbuf = com.readAll();

    if ((Rbuf.size() != 1 )||(Rbuf[0]!='L'))
    {
        str = "\rОшибка обмена ";
        PrintMess(str);

        com.close();
        Red_lamp_232();
        off_prg();
        setRrBarNull();
        return;
    }

    qDebug() << "Resive!";
    qDebug() << Rbuf;
    qDebug() << Rbuf.size();

    Rbuf.clear();
    Wbuf.clear();
    AddtoBar(4);
    com.write((LPSTR)(bufram+dwadrboot),ilboot);
    com.flush();
    com.waitForReadyRead(400);
    Rbuf = com.readAll();
    qDebug() << "Resive!";
    qDebug() << Rbuf;
    qDebug() << Rbuf.size();

    if ((Rbuf.size() !=1 )||(Rbuf[0]!='K'))
    {

        str = "\rОшибка обмена ";
        PrintMess(str);

        com.close();
        Red_lamp_232();
        off_prg();
        setRrBarNull();
        return;
    }
    Rbuf.clear();
    Wbuf.clear();
    for(int i=0;i<(ilboot>>3);i++)
        {
            Wbuf[0] = 'Y';
            Wbuf[1] = (dwadrboot+8*i) & 0xff;
            Wbuf[2] = ((dwadrboot+8*i)>>8) & 0xff;
            Wbuf[3] = 0;
            Wbuf[4] = 0x20;
            Wbuf[5] = 8;
            Wbuf[6] = 0;
            Wbuf[7] = 0;
            Wbuf[8] = 0;
            com.write(Wbuf,9);
            com.flush();
            f=TRUE;
            Sleep(10);
            com.waitForReadyRead(1500);
            Rbuf = com.readAll();
            qDebug() << "Resive!";
            qDebug() << Rbuf;
            qDebug() << Rbuf.size();

            if ((Rbuf.size() ==10 )&&(Rbuf[0]=='Y')&&(Rbuf[9]=='K'))

            {
                for(int j=0;j<8;j++)
                            {
                                if(Rbuf[j+1] != (char)bufram[dwadrboot+8*i+j])
                                f= FALSE;
                            }
             }
             else
             f= FALSE;
             if (!f)
             {
                 str = "\rОшибка обмена ";
                 PrintMess(str);

                 com.close();
                 Red_lamp_232();
                 off_prg();
                 setRrBarNull();
                 return;
             }
        }
    Rbuf.clear();
    Wbuf.clear();
    Wbuf[0] = 'R';
    Wbuf[1] = dwadrboot & 0xff;
    Wbuf[2] = (dwadrboot>>8) & 0xff;
    Wbuf[3] = 0;
    Wbuf[4] = 0x20;
    com.write(Wbuf,5);
    com.flush();
    com.waitForReadyRead(500);
    Rbuf = com.readAll();
    qDebug() << "Resive!";
    qDebug() << Rbuf;
    qDebug() << Rbuf.size();
    if((Rbuf.size() !=1)||(Rbuf[0]!='R'))
    {
        str = "\rОшибка обмена ";
        PrintMess(str);

        com.close();
        Red_lamp_232();
        off_prg();
        setRrBarNull();
        return;

    }

    Rbuf.clear();
    Wbuf.clear();
    Wbuf[0] = 'I';
    com.write(Wbuf,1);
    com.flush();
    qDebug() << "SEND";
    qDebug() << Wbuf;
    qDebug() << Wbuf.size();
     AddtoBar(5);
    com.waitForReadyRead(1700);
    com.waitForReadyRead(1700);
    com.waitForReadyRead(1700);
    Sleep(500);
    com.waitForReadyRead(1700);
    com.waitForReadyRead(1700);
    Rbuf = com.readAll();
    qDebug() << "Resive!";
    qDebug() << Rbuf;
    qDebug() << Rbuf.size();
    f=TRUE;

    if (Rbuf.size() ==37)
   //if (Rbuf.size() ==12)
    {
        str=Rbuf;
        PrintMess2(str);

        for(int j=0;j<12;j++)
                    {
                        if(Rbuf[j] != id_str[j])
                            f= FALSE;
                        //PrintMess1("Rbuf[j] "+ Rbuf[j] +" idstr "+ id_str[j] );
                    }

    }
    else
        f= FALSE;
    if (!f)
    {
        str = "\rОшибка идентификации загрузчика ";
        PrintMess(str);
        com.close();
        Red_lamp_232();
        off_prg();
        setRrBarNull();
        return;
    }

    str = " ...OK";
    PrintMess(str);
    //com.close();
   // Sleep(300);
    AddtoBar(6);

    if ((program == 1)||(verify == 1 ))
    {
        GetHexCod(dir);


    }

    Sleep(400);




    if (erase ==1)                               //галочка очистить
    {   DWORD	adr,data;
        str = "Очистка...";
        PrintMess1(str);
        Wbuf.clear();
        Rbuf.clear();

        Wbuf[0] = 'E';
        com.write(Wbuf,1);
        com.flush();
        qDebug() << "SEND";
        qDebug() << Wbuf;
        qDebug() << Wbuf.size();
        Sleep(2000);
        com.waitForReadyRead(2000);
        Rbuf = com.readAll();
        qDebug() << "Resive!";
        qDebug() << Rbuf;
        qDebug() << Rbuf.size();
        if((Rbuf.size() !=9)||(Rbuf[0]!='E'))
        {
            str = "\rОшибка обмена ";
            PrintMess(str);
            PrintMess( "Rbuf.size " + QString::number(Rbuf.size())  + " Rbuf " + Rbuf);
            com.close();
            Red_lamp_232();
            off_prg();
            setRrBarNull();
            return;

        }
        adr = (BYTE)Rbuf[1] + (((BYTE)Rbuf[2])<<8) + (((BYTE)Rbuf[3])<<16)
                + (((BYTE)Rbuf[4])<<24);
        data = (BYTE)Rbuf[5] + (((BYTE)Rbuf[6])<<8) + (((BYTE)Rbuf[7])<<16)
                + (((BYTE)Rbuf[8])<<24);
            if((adr == 0x08020000)&&(data == 0xffffffff))
            {
                str = " OK";
                PrintMess(str);

            }
            else
            {
            str = "\rОшибка очистки";
            PrintMess(str);
            com.close();
            Red_lamp_232();
            off_prg();
            setRrBarNull();
            return;
            }
    }

    if (eraseAll ==1)                      //галочка очистить все
    {
        DWORD	adr,data;
        str = "Полная очистка...";
        PrintMess1(str);
        Wbuf[0] = 'e';
        com.write(Wbuf,1);
        com.flush();
        qDebug() << "SEND";
        qDebug() << Wbuf;
        qDebug() << Wbuf.size();
        Sleep(1500);
        com.waitForReadyRead(2500);
        com.waitForReadyRead(2500);
        Rbuf = com.readAll();
        qDebug() << "Resive!";
        qDebug() << Rbuf;
        qDebug() << Rbuf.size();

        if((Rbuf.size() !=9)||(Rbuf[0]!='e'))
        {
            str = "\rОшибка обмена ";
            PrintMess(str);
            PrintMess( "Rbuf.size " + QString::number(Rbuf.size())  + " Rbuf " + Rbuf);
            com.close();
            Red_lamp_232();
            off_prg();
            setRrBarNull();
            return;
        }
        adr = (BYTE)Rbuf[1] + (((BYTE)Rbuf[2])<<8) + (((BYTE)Rbuf[3])<<16)
                + (((BYTE)Rbuf[4])<<24);
        data = (BYTE)Rbuf[5] + (((BYTE)Rbuf[6])<<8) + (((BYTE)Rbuf[7])<<16)
                + (((BYTE)Rbuf[8])<<24);
            if((adr == 0x08020000)&&(data == 0xffffffff))
            {
                str = " OK";
                PrintMess(str);

            }
            else
            {
            str = "\rОшибка полной очистки ";
            PrintMess(str);
            com.close();
            Red_lamp_232();
            off_prg();
            setRrBarNull();
            return;
            }




    }



    if (program ==1)                     //галочка программировать

    {
        BYTE ks;
        int i,j;
        str = "Программирование "+QString::number(ilcod) + " байт..." ;
        PrintMess1(str);

        Wbuf.clear();
        Rbuf.clear();

        Wbuf[0] = 'A';
        Wbuf[1] = 0x00;
        Wbuf[2] = 0x00;
        Wbuf[3] = 0x00;
        Wbuf[4] = 0x08;
        qDebug() << "SEND";
        qDebug() << Wbuf;
        qDebug() << Wbuf.size();
        com.write(Wbuf,5);
        com.flush();

        com.waitForReadyRead(1600);
        Rbuf = com.readAll();
        qDebug() << "Resive!";
        qDebug() << Rbuf;
        qDebug() << Rbuf.size();
        if((Rbuf.size() !=1)||(Rbuf[0]!=0x08))
        {
            str = "\rОшибка обмена1 ";
            PrintMess(str);
            PrintMess( "Rbuf.size " + QString::number(Rbuf.size())  + " Rbuf " + Rbuf);
            com.close();
            Red_lamp_232();
            off_prg();
            setRrBarNull();
            return;
        }

        Wbuf.clear();
        Rbuf.clear();

        Wbuf[0] = 'P';
        SetMaxBar((ilcod>>8));

        for (i= 0; i <(ilcod>>8);i++)
        {
            AddtoBar(i);
            com.write(Wbuf,1);
            com.flush();
            qDebug() << "SEND";
            qDebug() << Wbuf;
            qDebug() << Wbuf.size();
            com.write((LPSTR)(bufcod+(i<<8)),256);
            com.flush();

             ks=0;
            for (j = 0; j<256; j++)
            ks += bufcod[j+(i<<8)];

            com.waitForReadyRead(400);
            Rbuf = com.readAll();
            qDebug() << "Resive!";
            qDebug() << Rbuf;
            qDebug() << Rbuf.size();
            qDebug() << ks;

            if((Rbuf.size() !=1)||((BYTE)Rbuf[0]!=ks))
            {
                str = "\rОшибка обмена2 ";
                PrintMess(str);
                PrintMess( "Rbuf.size " + QString::number(Rbuf.size())  + " Rbuf " + Rbuf);
                com.close();
                Red_lamp_232();
                off_prg();
                setRrBarNull();
                return;
            }
        }
        str = " OK";
        PrintMess(str);




    }

    if (verify ==1)                       //галочка проверки
    {   QByteArray temp,temp1,temp2;
        int i,j,k=0;

        str = "Проверка..";
        PrintMess1(str);
        Wbuf.clear();
        Rbuf.clear();
        Wbuf[0] = 'A';
        Wbuf[1] = 0x00;
        Wbuf[2] = 0x00;
        Wbuf[3] = 0x00;
        Wbuf[4] = 0x08;
        com.write(Wbuf,5);
        com.flush();

        com.waitForReadyRead(400);
        Rbuf = com.readAll();
        if((Rbuf.size() !=1)||(Rbuf[0]!=0x08))
        {
            str = "\rОшибка обмена ";
            PrintMess(str);
            PrintMess( "Rbuf.size " + QString::number(Rbuf.size())  + " Rbuf " + Rbuf);
            com.close();
            Red_lamp_232();
            off_prg();
            setRrBarNull();
            return;

        }
        Wbuf.clear();
        Rbuf.clear();
        Wbuf[0] = 'V';
        SetMaxBar((ilcod>>8));

        for(i=0;i<(ilcod>>8);i++)
            {   AddtoBar(i);
                for(j=0;j<32;j++)

                {




                    Rbuf.clear();
                    temp.clear();
                    temp1.clear();
                    temp2.clear();

                        com.write(Wbuf,1);
                        com.flush();
                        qDebug() << "SEND";
                        qDebug() << Wbuf;
                        qDebug() << Wbuf.size();

                        com.waitForReadyRead(300); //374 i на 70
                        Rbuf = com.readAll();
                        qDebug() << "Resive!";
                        qDebug() << Rbuf;
                        qDebug() << Rbuf.size();
                        qDebug() << i;
                        if (Rbuf.size() == 8)
                        {
                            for(k=0;k<8;k++)
                                 {
                                if((BYTE)Rbuf[k] != bufcod[k+(j<<3)+(i<<8)])
                                      {
                                        str = "\rОшибка проверки!";
                                        PrintMess(str);
                                        com.close();
                                        Red_lamp_232();
                                        off_prg();
                                        setRrBarNull();
                                        return;
                                      }
                                 }
                        }

                        else
                        {   if (Rbuf.size() != 8)
                            {
                            temp = Rbuf;
                            com.write(Wbuf,1);
                            com.flush();
                            qDebug() << "TEMPPPP";
                            qDebug() << temp;
                            qDebug() << temp.size();

                            com.waitForReadyRead(300); //374 i на 70
                            Rbuf = com.readAll();
                            qDebug() << "Resive!";
                            qDebug() << Rbuf;
                            qDebug() << Rbuf.size();
                             qDebug() << i;
                            if (Rbuf.size() < 8)
                             {
                                temp+=Rbuf;
                                qDebug() << "TEMPPPP";
                                qDebug() << temp;
                                qDebug() << temp.size();
                                if (temp.size() == 8)
                                {
                                    for(k=0;k<8;k++)
                                        {
                                                if((BYTE)temp1[k] != bufcod[k+(j<<3)+(i<<8)])
                                            {
                                                    str = "\rОшибка проверки!";
                                                    PrintMess(str);
                                                    com.close();
                                                    Red_lamp_232();
                                                    off_prg();
                                                    setRrBarNull();
                                                    return;
                                            }
                                         }
                                }
                             }
                             else
                                {
                                temp+=Rbuf;
                                temp1[0] = temp[0];temp2[0] = temp[8];
                                temp1[1] = temp[1];temp2[1] = temp[9];
                                temp1[2] = temp[2];temp2[2] = temp[10];
                                temp1[3] = temp[3];temp2[3] = temp[11];
                                temp1[4] = temp[4];temp2[4] = temp[12];
                                temp1[5] = temp[5];temp2[5] = temp[13];
                                temp1[6] = temp[6];temp2[6] = temp[14];
                                temp1[7] = temp[7];temp2[7] = temp[15];
                                for(k=0;k<8;k++)
                                    {
                                            if((BYTE)temp1[k] != bufcod[k+(j<<3)+(i<<8)])
                                        {
                                                str = "\rОшибка проверки!";
                                                PrintMess(str);
                                                com.close();
                                                Red_lamp_232();
                                                off_prg();
                                                setRrBarNull();
                                                return;
                                        }
                                     }
                                j++;
                                for(k=0;k<8;k++)
                                    {
                                            if((BYTE)temp2[k] != bufcod[k+(j<<3)+(i<<8)])
                                        {
                                                str = "\rОшибка проверки!";
                                                PrintMess(str);
                                                com.close();
                                                Red_lamp_232();
                                                off_prg();
                                                setRrBarNull();
                                                return;
                                        }
                                     }

                                }
                            }


                        }




                }

               }

        str = " OK";
        PrintMess(str);

    }
com.close();
Red_lamp_232();
off_prg();
setRrBarNull();
}



BOOL PotokThread::GetBootCod(void)    //получение данных для заливки загрузчика
{
int i,nb;

QString strfn;
char chd;
DWORD dwadr;

   dwadr_seg_hex = 0;
   dwadr_lineoffs_hex = 0;

   for(i=0; i<sizeof(bufram);i++)
   {
       bufram[i] = 0xff;
   }
   strfn = "\\K4000_BOOT_CP.HEX";
   //strfn = "\\1986_BOOT_UART.HEX";
   strfn = QDir::currentPath() + strfn;
   QFile file(strfn);
   qDebug () <<strfn;


   file.open(QIODevice::ReadOnly);
   if (file.isOpen())

   nb=1;
   while(nb == 1)
       {
           i = 0;
           do
           {
               nb = file.read(&chd,1);
               buf_hexstr[i] = chd;
               i++;

           }
           while(chd!='\n');
           if(nb != 1)
                   {
                       file.close();
                       for(i=0;i<sizeof(bufram);i++)
                       {
                           if(bufram[i] != 0xff)
                               break;
                       }
                       dwadrboot = i;
                       for(i=(sizeof(bufram)-1);i>=0;i--)
                       {
                           if(bufram[i] != 0xff)
                               break;
                       }
                       ilboot = (i+16 - dwadrboot) & 0xfffffff7;

                       return 1;
                   }
           if(!GetDataHex())
                   {   file.close();
                       return 0;
                   }
                   if(btype_hex == 0)
                   {
                       dwadr = dwadr_lineoffs_hex + dwadr_seg_hex + wadr_offs_hex;
                       if((dwadr<0x20000000)||((dwadr+bl_hex)>0x20008000))
                       {   qDebug () << "5";
                           file.close();
                           return 0;
                       }
                       dwadr -= 0x20000000;
                       for(i=0;i<bl_hex;i++)
                           bufram[dwadr+i] = buf_data_hex[i];
                   }
               }

               return 0;
}

BOOL PotokThread::GetHexCod(QString dir)    //получение данных для заливки прошивки
{
int i,nb;
QString strfn;

char chd;
DWORD dwadr;

dwadr_seg_hex = 0;
dwadr_lineoffs_hex = 0;

for(i=0; i<sizeof(bufcod);i++)
{
    bufcod[i] = 0xff;

}



QFile file;
file.setFileName(dir);

if(!file.open(QIODevice::ReadOnly))
    return 0;

nb = 1;
    while(nb == 1)
    {
        i = 0;
        do
        {
            nb = file.read(&chd,1);
            buf_hexstr[i] = chd;
            i++;
        }
        while(chd!='\n');
        if(nb != 1)
        {
            file.close();
            for(i=(sizeof(bufcod)-1);i>=0;i--)
            {
                if(bufcod[i] != 0xff)
                    break;
            }
            if(i & 0xff)
                i = ((i + 0x100) & 0xffffff00);
            ilcod = i;
            return 1;
        }
        if(!GetDataHex())
        {
            file.close();
            return 0;
        }
        if(btype_hex == 0)
        {
            dwadr = dwadr_lineoffs_hex + dwadr_seg_hex + wadr_offs_hex;
            if((dwadr<0x08000000)||((dwadr+bl_hex)>0x08020000))
            {
                file.close();
                return 0;
            }
            dwadr -= 0x08000000;
            for(i=0;i<bl_hex;i++)
                bufcod[dwadr+i] = buf_data_hex[i];
        }
    }
    return 0;
}

BYTE PotokThread::GetHexByte(int ibuf)
{
    BYTE bh,bl;

        bh = buf_hexstr[ibuf] - 0x30;
        if (bh>9)
            bh -= 7;
        bh <<= 4;
        bl = buf_hexstr[ibuf+1] - 0x30;
        if (bl>9)
            bl -= 7;
        return bh+bl;
}
BOOL PotokThread::GetDataHex()
{
int i;
BYTE ks;
    if(buf_hexstr[0] != 0x3a)
        return	0;
    ks = 0;
    bl_hex = GetHexByte(1);
    wadr_offs_hex = ((WORD)GetHexByte(3)<<8)+GetHexByte(5);
    btype_hex = GetHexByte(7);
    ks = bl_hex + btype_hex + (wadr_offs_hex>>8) + wadr_offs_hex;
    for(i=0;i<bl_hex+1;i++)
    {
        buf_data_hex[i] = GetHexByte(2*i+9);
        ks += buf_data_hex[i];
    }
    if(ks!=0)
        return 0;
    if(btype_hex == 2)
        dwadr_seg_hex =	(((WORD)GetHexByte(9)<<8)+GetHexByte(11))<<4;
    if(btype_hex == 4)
        dwadr_lineoffs_hex = (((WORD)GetHexByte(9)<<8)+GetHexByte(11))<<16;
    return 1;
}
void PotokThread::SetParam(bool e, bool eA, bool p, bool v)  //передача параметров галочек
{

     erase=e;
     eraseAll=eA;
     program=p;
     verify=v;
}


void PotokThread::SetIpMac(QString ip,QString mac1,QString mac2,QString dir) //кнопка записать в hex
{
    QFile FileHex;
    QString strToHexFile;
    QString temp;
    QByteArray tempb;
    char  tempc[2];
    BYTE CRC;
    byte const0xFF = 0x00;
    int tempi;
    temp.clear();
    FileHex.setFileName(dir);

    if(!FileHex.open(QIODevice::ReadOnly))
    {
        str="Ошибка открытия Hex-файла";
        PrintMess(str);
        return;
   }

   for (int i = 0; i<ip.size()+1; i++)
   {
       if ((ip[i] != '.' )&&(i != ip.size()))
       {
           temp+=ip[i];

       }
       else
       {
           if (temp.size()==0)
           {
                str="Не все поля IP адреса заполнены";
                PrintMess(str);
                return;
           }
       temp.clear();
       }

   }
   if(mac1.size()!=17)
   {
       str="Не заполнен 1 MAC адресс";
       PrintMess(str);
       return;
   }
   if(mac2.size()!=17)
   {
       str="Не заполнен 2 MAC адресс";
       PrintMess(str);
       return;
   }

   temp.clear();
   strToHexFile = ":10F00000";  //начали формировать строку для записи в 16ти ричной форме
                                //значение  мак адресов переписываются сразу + форомируется добавочный CRC код
   strToHexFile+= mac1[0];
   strToHexFile+= mac1[1];
   temp+= mac1[0];
   temp+= mac1[1];
   tempb = temp.toLocal8Bit();
   tempc[0] = tempb[0];
   tempc[1] = tempb[1];
   CRC= const0xFF-CharHexToBYTE(tempc);
   temp.clear();

   strToHexFile+= mac1[3];
   strToHexFile+= mac1[4];
   temp+= mac1[3];
   temp+= mac1[4];
   tempb = temp.toLocal8Bit();
   tempc[0] = tempb[0];
   tempc[1] = tempb[1];
   CRC= CRC-CharHexToBYTE(tempc);
   temp.clear();

   strToHexFile+= mac1[6];
   strToHexFile+= mac1[7];   
   temp+= mac1[6];
   temp+= mac1[7];
   tempb = temp.toLocal8Bit();
   tempc[0] = tempb[0];
   tempc[1] = tempb[1];
   CRC= CRC-CharHexToBYTE(tempc);
   temp.clear();

   strToHexFile+= mac1[9];
   strToHexFile+= mac1[10];   
   temp+= mac1[9];
   temp+= mac1[10];
   tempb = temp.toLocal8Bit();
   tempc[0] = tempb[0];
   tempc[1] = tempb[1];
   CRC= CRC-CharHexToBYTE(tempc);
   temp.clear();

   strToHexFile+= mac1[12];
   strToHexFile+= mac1[13];
   temp+= mac1[12];
   temp+= mac1[13];
   tempb = temp.toLocal8Bit();
   tempc[0] = tempb[0];
   tempc[1] = tempb[1];
   CRC= CRC-CharHexToBYTE(tempc);
   temp.clear();

   strToHexFile+= mac1[15];
   strToHexFile+= mac1[16];
   temp+= mac1[15];
   temp+= mac1[16];
   tempb = temp.toLocal8Bit();
   tempc[0] = tempb[0];
   tempc[1] = tempb[1];
   CRC= CRC-CharHexToBYTE(tempc);
   temp.clear();

   strToHexFile+= mac2[0];
   strToHexFile+= mac2[1];
   temp+= mac2[0];
   temp+= mac2[1];
   tempb = temp.toLocal8Bit();
   tempc[0] = tempb[0];
   tempc[1] = tempb[1];
   CRC= CRC-CharHexToBYTE(tempc);
   temp.clear();

   strToHexFile+= mac2[3];
   strToHexFile+= mac2[4];
   temp+= mac2[3];
   temp+= mac2[4];
   tempb = temp.toLocal8Bit();
   tempc[0] = tempb[0];
   tempc[1] = tempb[1];
   CRC= CRC-CharHexToBYTE(tempc);
   temp.clear();

   strToHexFile+= mac2[6];
   strToHexFile+= mac2[7];
   temp+= mac2[6];
   temp+= mac2[7];
   tempb = temp.toLocal8Bit();
   tempc[0] = tempb[0];
   tempc[1] = tempb[1];
   CRC= CRC-CharHexToBYTE(tempc);
   temp.clear();

   strToHexFile+= mac2[9];
   strToHexFile+= mac2[10];
   temp+= mac2[9];
   temp+= mac2[10];
   tempb = temp.toLocal8Bit();
   tempc[0] = tempb[0];
   tempc[1] = tempb[1];
   CRC= CRC-CharHexToBYTE(tempc);
   temp.clear();

   strToHexFile+= mac2[12];
   strToHexFile+= mac2[13];
   temp+= mac2[12];
   temp+= mac2[13];
   tempb = temp.toLocal8Bit();
   tempc[0] = tempb[0];
   tempc[1] = tempb[1];
   CRC= CRC-CharHexToBYTE(tempc);
   temp.clear();

   strToHexFile+= mac2[15];
   strToHexFile+= mac2[16];
   temp+= mac2[15];
   temp+= mac2[16];
   tempb = temp.toLocal8Bit();
   tempc[0] = tempb[0];
   tempc[1] = tempb[1];
   CRC= CRC-CharHexToBYTE(tempc);
   temp.clear();


   QString temp2;

   for (int i = 0; i<ip.size()+1; i++)  //разделяем  стоку ip на числа, преобразовываем в 16ти ричную
                                        //систему счисления формируем строку для записи в файл + CRC код
           {



            if ((ip[i] != '.' )&&(i != ip.size()))
            {
                temp+=ip[i];

            }
            else
            {
                qDebug() << "TEMP";
                qDebug() << temp;

                tempi= temp.toInt();
                 CRC= CRC- temp.toInt();
                qDebug() << tempi;

                temp2.append(QString("%1").arg(tempi,0,16).toUpper());
                if (temp2.size()==1)
                {qDebug() << "+0 CoOOL";
                strToHexFile+="0";

                }

                qDebug() << "TEMP2";
                qDebug() << temp2;
                strToHexFile+=temp2;
                temp.clear();
                temp2.clear();

            }


           }




   CRC = CRC - 0x10 - 0xF0;
   int k = CRC;
   if ( k ==0 )
   strToHexFile+=QString("%1").arg(k,0,16).toUpper();
   strToHexFile+=QString("%1").arg(k,0,16).toUpper();
   strToHexFile+=":00000001FF\r\n";
  qDebug() <<strToHexFile;


  QFile file(dir);
  QString line;
  qint64 len = 0;
  tempb.clear();
  tempb = strToHexFile.toLocal8Bit();
  if (file.open(QIODevice::ReadWrite))
  {
      QTextStream t(&file);
      while(!t.atEnd())
      {
          line = t.readLine();
      }
      len =t.pos()-line.length()-2;


             t.seek(len);
             file.write(tempb,tempb.size());
             file.flush();


  }
  file.close();
  Sleep(500);
  str="Удачно записано в файл:";
  PrintMess(str);
  PrintMess("IP " + ip);
  PrintMess("MAC1 " + mac1);
  if (mac2!="00:00:00:00:00:00") PrintMess("MAC2 " +mac2);

}


BOOL PotokThread::CharHexToBYTE(char Str[2]) //преобразования для записи в файл маков и айпи
{
BYTE value, tmpValue1, tmpValue2;
    QString strBuff;
    value = 0;
    tmpValue1 = 0;
    tmpValue2 = 0;
    switch (Str[0])
        {
            case '0':
                tmpValue1 = 0x0; break;
            case '1':
                tmpValue1 = 0x1; break;
            case '2':
                tmpValue1 = 0x2; break;
            case '3':
                tmpValue1 = 0x3; break;
            case '4':
                tmpValue1 = 0x4; break;
            case '5':
                tmpValue1 = 0x5; break;
            case '6':
                tmpValue1 = 0x6; break;
            case '7':
                tmpValue1 = 0x7; break;
            case '8':
                tmpValue1 = 0x8; break;
            case '9':
                tmpValue1 = 0x9; break;
            case 'A':
                tmpValue1 = 10; break;
            case 'B':
                tmpValue1 = 11; break;
            case 'C':
                tmpValue1 = 12; break;
            case 'D':
                tmpValue1 = 13; break;
            case 'E':
                tmpValue1 = 14; break;
            case 'F':
                tmpValue1 = 15; break;



        }


        switch (Str[1])
        {
            case '0':
                tmpValue2 = 0; break;
            case '1':
                tmpValue2 = 1; break;
            case '2':
                tmpValue2 = 2; break;
            case '3':
                tmpValue2 = 3; break;
            case '4':
                tmpValue2 = 4; break;
            case '5':
                tmpValue2 = 5; break;
            case '6':
                tmpValue2 = 6; break;
            case '7':
                tmpValue2 = 7; break;
            case '8':
                tmpValue2 = 8; break;
            case '9':
                tmpValue2 = 9; break;
            case 'A':
                tmpValue2 = 10; break;
            case 'B':
                tmpValue2 = 11; break;
            case 'C':
                tmpValue2 = 12; break;
            case 'D':
                tmpValue2 = 13; break;
            case 'E':
                tmpValue2 = 14; break;
            case 'F':
                tmpValue2 = 15; break;


        }
        value = tmpValue1 * 16 + tmpValue2;
        return value;

}

void PotokThread::StartTest(QString com1,QString com2 )   //кнопка запустить
{

     on_test();
    ComRs485=com2;
    ClosePort232();
    Sleep(300);

   StartPort232(com1,QSerialPort::Baud115200,QSerialPort::NoParity);

    QSerialPort rs485;

    rs485.setPortName(com2);
    rs485.setBaudRate(QSerialPort::Baud115200);
    rs485.setDataBits(QSerialPort::Data8);
    rs485.setParity(QSerialPort::NoParity);
    rs485.setStopBits(QSerialPort::OneStop);
    rs485.setFlowControl(QSerialPort::NoFlowControl);
    rs485.open(QIODevice::ReadWrite);


   if (rs485.isOpen())
       {
       Green_lamp_485();


       }
   else
   {
       str="Ошибка открытия COM порта RS485";
       PrintMess(str);
       off_test();
       return;
   }
   rs485.close();
   Red_lamp_485();
   //Sleep(2500);
   StartTest();
   Sleep(1500);




}
void PotokThread::TestRs485()
{
    QSerialPort rs485;
    QByteArray Wbuf,data;
    rs485.setPortName(ComRs485);
    rs485.setBaudRate(QSerialPort::Baud115200);
    rs485.setDataBits(QSerialPort::Data8);
    rs485.setParity(QSerialPort::NoParity);
    rs485.setStopBits(QSerialPort::OneStop);
    rs485.setFlowControl(QSerialPort::NoFlowControl);
    rs485.open(QIODevice::ReadWrite);

    Sleep(800);
   if (rs485.isOpen())
       {
       Green_lamp_485();
       str= "Порт RS485 открыт ";
       PrintMess(str);
       }
   else
   {
       str="Ошибка открытия COM порта RS485";
       PrintMess(str);
       off_test();
       return;
   }
   Sleep(1500);
   rs485.waitForReadyRead(2000);
   data = rs485.readAll();
   if (data=="QWERTY")
   {
      Wbuf[0] = 'Z';
      Wbuf[1] = 'X';
      Wbuf[2] = 'C';
      Wbuf[3] = 'V';
      Wbuf[4] = 'B';
      Wbuf[5] = 'N';
      Wbuf[6] = 'M';
      rs485.write(Wbuf,7);
      rs485.flush();
   }
   rs485.close();
   qDebug() <<data;
   str= "Порт RS485 закрыт ";
   PrintMess(str);
   Red_lamp_485();
   off_test();
}


void PotokThread::Ping()
{
    QProcess ping1,ping2;
    QString temp1 ="10.20.10.100" ,temp2 ="10.20.20.101";

    for (int i = 0; i<10; i++)
    {

     ping1.start("ping",QStringList() << temp1);
     ping2.start("ping",QStringList() << temp2);
     ping1.waitForFinished(500);
     ping2.waitForFinished(500);
     ping1.close();
     ping2.close();
     Sleep(200);
    }

    // Sleep(2500);
    ping1.start("ping",QStringList() << temp1);
    ping2.start("ping",QStringList() << temp2);
    ping1.waitForFinished(500);
    ping2.waitForFinished(500);
    ping1.close();
    ping2.close();
}








