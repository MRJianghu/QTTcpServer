#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <work.h>
#include <QThread>
#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QStatusBar *m_statusBar;
    void OnFileOpenError();

private slots:
    void on_setFile_2_clicked();

    void on_setFile_clicked();

    void on_send_clicked();

    void setValue(int value);

signals:
    void startConnect(QString,unsigned short);
    void sendSignal(QString path);
private:
    Ui::Widget *ui;

};
#endif // WIDGET_H
