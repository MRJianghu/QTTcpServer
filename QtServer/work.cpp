#include "work.h"

Work::Work(qintptr socketDescriptor, QObject *parent)
    : QObject{parent}
{
    num = 1;
    n = 0;
    m_socket = new QTcpSocket;
    m_socket->setReadBufferSize(8129);
    myBuffer.resize(0);
    m_socket->setSocketDescriptor(socketDescriptor);
    connect(m_socket, &QTcpSocket::readyRead, this, &Work::OnReadyRead);
    connect(m_socket, &QTcpSocket::connected, this, &Work::OnConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &Work::OndisConnected);
}


void Work::save(const QByteArray content)
{
    file->open(QIODevice::ReadWrite | QIODevice::Append);
    if(file->isOpen())
    {
        file->write(content);
        qInfo() << "写入完毕!";
    }
    else
    {
        qInfo() << "文件不能打开";
    }
    file->close();
    delete file;
}

void Work::openFile(const char *type)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "\\save." +type;
    qInfo() << path;
    file = new QFile(path);
    file->open(QIODevice::ReadWrite);
    file->close();
}



void Work::OnReadyRead()
{
    qInfo() << "触发";
     if(m_socket->bytesAvailable() <= 0)
         return;                                        
     QByteArray buffer(m_socket->readAll());
     myBuffer.append(buffer);

    //获取文件大小
    if(num == 1)
    {
        memcpy(&len, myBuffer.data(), sizeof(int));    //数据长度
        myBuffer.remove(0, sizeof(int));
        qInfo() << "传来的文件的大小 ："<<len;
        num = 0;
    }
    qInfo() << "myBuffer size" << myBuffer.size() <<"要求大小" <<len;
    if(myBuffer.size() < len)
    {
        return;
    }
    else
    {
        qInfo() << "ok";
        content.append(myBuffer);
        myBuffer.remove(0,len);
        num = 2;
    }

    //类型
   if(num == 2)
   {
       if(myBuffer.size() < sizeof(int))
       {
           return;
       }
       else
       {
           memcpy(&len, myBuffer.data(), sizeof(int));    //数据长度
           qInfo() << len;
           myBuffer.remove(0, sizeof(int));
       }

       if(myBuffer.size() < len)
       {
           return;
       }
       else
       {
           char* type = new char[len + 1];
           memcpy(type, myBuffer.data(), len);
           myBuffer.remove(0, len);
           type[len] = '\0';
           qInfo() << type;
           openFile(type);
           delete[] type;
       }
       save(content);
   }

}

void Work::OnConnected()
{
    qInfo() << "已连接客服端：" << m_socket->peerName() << " 地址：" << m_socket->peerAddress()
            <<" 端口：" <<m_socket->peerPort();
}

void Work::OndisConnected()
{
    qInfo() << "客服端：" << m_socket->peerName() << "以下线 地址：" << m_socket->peerAddress()
            <<" 端口：" <<m_socket->peerPort();
    emit Over();
    m_socket->close();
    m_socket->deleteLater();
}
