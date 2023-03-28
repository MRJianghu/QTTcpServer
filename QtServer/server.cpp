#include "server.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{

}

Server::~Server()
{
}

void Server::showCilent(QTcpSocket* socket)
{
    qInfo() << "客服姓名: \n" << socket->peerName()
            << "客服地址: \n" << socket->peerAddress()
            << "客服端口：\n" << socket->peerPort();
}


void Server::incomingConnection(qintptr handle)
{
    m_socket = new QTcpSocket();
    m_socket->setSocketDescriptor(handle);
    clientAddress = m_socket->peerAddress().toString();
    qInfo() <<"有新的连接！";
    myThread* thread = new myThread(m_socket,this);

    connect(thread, &myThread::runOver, this, [=](){

        qInfo() << "地址:" << clientAddress <<"发来的文件已经接收完毕！";
    });

    connect(m_socket, &QTcpSocket::disconnected, this, [=](){
        qInfo() <<"地址:" <<clientAddress <<"已下线！";
                thread->quit();
                thread->wait();
                thread->deleteLater();
                m_socket->close();
                m_socket->deleteLater();
    });
    thread->start();
}
