#ifndef  _TCP_SERVER_H_
#define  _TCP_SERVER_H_
#include <iostream>
#include "Receiver.h"
#include "Ipv4Address.h"
#include "Env.h"
class TcpServer
{
public:
    virtual ~TcpServer();

    void start();
protected:
    TcpServer(Env* env,const Ipv4Address& addr);
    virtual void handleNewConnection(ClientInfo* clientinfo) = 0;
private:
    static void newConnectionCallback(void * arg,ClientInfo* ClientInfo);

protected:
    Env* mEnv;
    Receiver* mReceiver;
    Ipv4Address mAddr;
};

#endif