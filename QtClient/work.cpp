#include "work.h"

Work::Work(QObject *parent)
    : QObject{parent}
{

}

Work::~Work()
{
    free(m_tcp);
}

void Work::connectServer(QString ip, unsigned short port)
{
    m_tcp = new QTcpSocket;
    m_tcp->connectToHost(QHostAddress(ip), port);

    connect(m_tcp, &QTcpSocket::connected, this, &Work::connectOk);
    connect(m_tcp, &QTcpSocket::disconnected, this,[=](){
        m_tcp->close();
        m_tcp->deleteLater();
        emit dropConnect();
    });
}

void Work::send(QString path)
{
    qInfo() << path;
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        QFileInfo fileInfo(path);
        //获取文件大小
        long long fileSize = fileInfo.size();
        qInfo() << "文件大小 " << fileSize;
        char* file_size = (char *) malloc(sizeof(char) * sizeof(long long));
        memcpy(file_size, &fileSize, sizeof(long long));

        //获取文件名称
        QString fileName = fileInfo.fileName();
        QByteArray name = fileName.toUtf8();
        int nameSize = name.size();
        char* name_size = (char*) malloc (sizeof(char) * sizeof(int));
        memcpy(name_size, &nameSize, sizeof(int));
        long long totalSize = sizeof(long long) + sizeof(int) + fileSize + nameSize;


        //写数据长度
        long long num = 0;
        int currentPercent = 0;

        num += m_tcp->write(file_size, sizeof(long long));
        qInfo() << num;
        currentPercent = (num * 100) / totalSize;
        emit curPercent(currentPercent);
        //写数据

        while(!file.atEnd())
        {
            num += m_tcp->write(file.readLine());
            currentPercent = (num * 100) / totalSize;
            emit curPercent(currentPercent);
        }

        //写文件名长度
        num += m_tcp->write(name_size, sizeof(int));
        currentPercent = (num * 100) / totalSize;
        emit curPercent(currentPercent);

        //写文件名
        num += m_tcp->write(name);
        currentPercent = (num * 100) / totalSize;
        emit curPercent(currentPercent);
    }
    else
    {
        emit canNotOpenFile();
    }
    m_tcp->reset();
}

