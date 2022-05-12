#include "socketclient.h"

SocketClient::SocketClient(SOCKET mSocket,QWidget *parent) : QThread(parent)
{
    this->mSocket = mSocket;
}

void SocketClient::run(){
    char buf[1024] = {0};
    while(true){
        if(mIsSend){
            ::send(mSocket,mMsg.toUtf8().data(),mMsg.length() +1,0);
            int errorCode = WSAGetLastError();
            if( errorCode == WSAECONNABORTED){
                emit isClose();
                return;
            }
            int ret = ::recv(mSocket,buf,1024,0);
            if( ret == 0){
                emit isClose();
                return;
            }
            QString msg = QString("%1").arg(buf);
            emit isMsg(msg);
            mIsSend = false;
        }
    }

}
void SocketClient::dealSend(QString msg){
    mIsSend = true;
    mMsg = msg;
}
