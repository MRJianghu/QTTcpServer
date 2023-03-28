#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <QStandardPaths>
#include <server.h>

enum acceptState {FILENAME, FILECONTEXT, FILESIZE};

class myThread : public QThread
{
    Q_OBJECT
public:
    explicit myThread(QTcpSocket* socket,QObject *parent = nullptr);
    ~myThread();

public:
    void getFileSize(QByteArray byteArray);
    void getFileTypeSize(QByteArray byteArray);
    void save(const QByteArray content, const char* name);
    void openFile(const char* fileName);
public:
    long long len_fileSize;
    int len_fileName;
    acceptState state;
    QFile* file;
    QTcpSocket* m_socket;
    QByteArray myBuffer;
    QByteArray content;

signals:
    void runOver();

protected:
    void run()override;
public slots:
    void OnReadyRead();
};

#endif // MYTHREAD_H
