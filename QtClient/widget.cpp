#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_statusBar = new QStatusBar(this);

    ui->ip->setText("127.0.0.1");
    ui->Port->setText("8022");

    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);

    //创建子进程
    QThread* t = new QThread;
    Work* worker = new Work;
    worker->moveToThread(t);
    connect(this,&Widget::startConnect,worker,&Work::connectServer);
    connect(worker, &Work::curPercent, this, &Widget::setValue);
    connect(worker, &Work::canNotOpenFile, this, &Widget::OnFileOpenError);
    connect(this,&Widget::sendSignal, worker, &Work::send);
    connect(worker, &Work::connectOk, this, [=](){
        QMessageBox::information(this,"连接到服务器","连接成功！");
    });
    connect(worker,&Work::dropConnect,worker,[=](){
        t->quit();
        t->wait();
        worker->deleteLater();
        t->deleteLater();
    });
    t->start();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::OnFileOpenError()
{
    QMessageBox::warning(this,"错误","选择的文件不能打开！");
}


void Widget::on_setFile_2_clicked()
{
    QString ip = ui->ip->text();
    unsigned short port = ui->Port->text().toUShort();
    emit startConnect(ip, port);
}


void Widget::on_setFile_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"选择文件");
    if(path.isEmpty())
    {
        QMessageBox::warning(this,"打开文件","选择的文件路径不能为空");
        return;
    }
    ui->filepath->setText(path);
    ui->progressBar->setValue(0);
}


void Widget::on_send_clicked()
{
    emit sendSignal(ui->filepath->text());
}


void Widget::setValue(int value)
{
    ui->progressBar->setValue(value);
}

