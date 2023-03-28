#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <mythread.h>
class Server : public QTcpServer
{
public:
    Server(QObject *parent = nullptr);
    ~Server();
    QTcpSocket* m_socket;
public:
    QString clientAddress;
public:
    static void showCilent(QTcpSocket* socket);
protected:
    void incomingConnection(qintptr handle) override;
};

#endif // SERVER_H
