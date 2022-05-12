#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <winsock2.h>
#include <Windows.h>
#include <QSettings>
#include <socketclient.h>
#include <reqwin.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
signals:
    void isSend(QString msg);

public slots:
    void dealMsg(QString msg);
    void send_message(QString data);
    void closeSocket();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void read_settings_ini();
    void fill_combobox(QString);
    void send_full_data();

private:
    Ui::Widget *ui;
    WSADATA mWasData;
    SOCKET mSocket;
    SocketClient* mSocketClient;
    QString mAddrStr;
    Reqwin *sWindow;
};
#endif // WIDGET_H
