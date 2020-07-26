#ifndef _RTSPSERVER__H
#define _RTSPSERVER__H

#include <iostream>
#include <string>
#include <list>
#include <thread>
#include <map>
#include "TcpServer.h"
#include "RtspConnection.h"
#include "MediaSession.h"
#include "Event.h"
#include "Env.h"
class RtspConnection;

class RtspServer :public TcpServer
{
public:
    static RtspServer* createNew(Env* env,Ipv4Address& addr);

    RtspServer(Env* env,const Ipv4Address& addr);
    virtual ~RtspServer();

     bool addMeidaSession(MediaSession* mediaSession);
     void handleNewConnection(int connfd);
     MediaSession* loopupMediaSession(std::string name);
     std::string getUrl(MediaSession* session);

protected:
    virtual void handleNewConnection(ClientInfo* clientinfo);
    static void disconnectionCallback(void* arg, int sockfd);
    void handleDisconnection(int sockfd);
    static void triggerCallback(void*);
    void handleDisconnectionList();

private:
    std::map<int, RtspConnection*> mConnections;
    std::map<std::string, MediaSession*> mMediaSessions;
    std::vector<int> mDisconnectionlist;
    TriggerEvent* mTriggerEvent;

};

#endif