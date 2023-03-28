#include "mythread.h"

myThread::myThread(QTcpSocket* socket,QObject *parent)
    : QThread{parent}
{
    state = FILESIZE;
    len_fileSize = len_fileName = 0;
    myBuffer.resize(0);
    m_socket = socket;
}

myThread::~myThread()
{
    free(file);
    free(m_socket);
}

void myThread::save(const QByteArray content, const char *name)
{
    openFile(name);
    file->open(QIODevice::ReadWrite | QIODevice::Append);
    if(file->isOpen())
    {
        file->write(content);
        qInfo() << "文件已保存至桌面，文件名称 " << name;
    }
    else
    {
        qInfo() << "文件存储失败，原因是文件未能打开该";
    }
    file->close();
}

void myThread::openFile(const char *fileName)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "\\" + fileName;
    file = new QFile(path);
    file->open(QIODevice::ReadWrite);
    if(file->isOpen())
    {
        qInfo() << "文件创建成功，文件名称" << path;
    }
    else
    {
        qInfo() << "文件创建失败" << path;
    }
    file->close();
}

void myThread::run()
{
    connect(m_socket, &QTcpSocket::readyRead, this, &myThread::OnReadyRead);
    exec();
}

void myThread::OnReadyRead()
{
    if(m_socket->bytesAvailable() <= 0)
        return;
    QByteArray buffer(m_socket->readAll());
    myBuffer.append(buffer);

    if(state == FILESIZE) {
        //获取文件大小数值的字节数
        if(myBuffer.size() < sizeof(long long))
            return;
        memcpy(&len_fileSize, myBuffer.data(), sizeof(long long));
        myBuffer.remove(0, sizeof(long long));
        qInfo() << "获取文件大小为："<<len_fileSize << "字节";
        state = FILECONTEXT;
    }

    if(state == FILECONTEXT) {
        if(myBuffer.size() < len_fileSize)
            return;
        content.append(myBuffer,len_fileSize);
        myBuffer.remove(0,len_fileSize);
        state = FILENAME;
    }

    if(state == FILENAME) {
        //获取文件名
        if(myBuffer.size() < sizeof(int))
            return;

        memcpy(&len_fileName, myBuffer.data(), sizeof(int));
        myBuffer.remove(0, sizeof(int));

        if(myBuffer.size() < len_fileName)
            return;
        char* name = new char[len_fileName + 1];
        memcpy(name, myBuffer.data(), len_fileName);
        name[len_fileName] = '\0';
        save(content, name);
        content.clear();
        emit runOver();
    }
    state = FILESIZE;
    len_fileSize = 0;
    len_fileName = 0;
    myBuffer.clear();
    myBuffer.resize(0);
}
