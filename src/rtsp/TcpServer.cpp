#include <assert.h>
#include <iostream>
#include "TcpServer.h"
#include <thread>
#include "Mylog.h"


TcpServer::TcpServer(Env* env,const Ipv4Address& addr) :
    mAddr(addr),
    mEnv(env)
{
    mReceiver = Receiver::createNew(addr);
    assert(mReceiver);
    mReceiver->setNewConnectionCallback(newConnectionCallback, this);
}
TcpServer::~TcpServer()
{
    delete mReceiver;
    mReceiver = nullptr;
}

void TcpServer::start()
{
    mReceiver->listen();
    mReceiver->doClient();
    mEnv->start();
    LOGI("TCP server start successfull\n");

}

void TcpServer::newConnectionCallback(void * arg,ClientInfo* info)
{
    TcpServer* tcpServer = (TcpServer*)arg;
    tcpServer->handleNewConnection(info);
}
