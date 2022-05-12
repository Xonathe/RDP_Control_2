#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QWidget>
#include <QThread>
#include <winsock2.h>

class SocketClient : public QThread
{
    Q_OBJECT
public:
    explicit SocketClient(SOCKET mSocket,QWidget *parent = nullptr);
    void run();

signals:
    void isMsg(QString msg);
    void isClose();

public slots:
    void dealSend(QString msg);
private:
    QString mMsg;
    SOCKET mSocket;
    bool mIsSend =false;
};

#endif // SOCKETCLIENT_H
