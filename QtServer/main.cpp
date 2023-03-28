#include <QCoreApplication>
#include <server.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("server");
    Server s;
    if (s.listen(QHostAddress::Any, 8022))
    {
        qInfo() << "成功监听8022端口";
    }
    else
    {
        qInfo() << "监听失败";
    }
    return a.exec();
}
