#ifndef WORK_H
#define WORK_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>
#include <QThread>

class Work : public QObject
{
    Q_OBJECT
public:
    explicit Work(QObject *parent = nullptr);
    ~Work();
    //连接服务器
    void connectServer(QString ip, unsigned short port);
    //发送文件
    void send(QString path);
signals:
    void connectOk();
    void dropConnect();
    void canNotOpenFile();
    void curPercent(long long percent);
private:
    QTcpSocket* m_tcp;
};

#endif // WORK_H
