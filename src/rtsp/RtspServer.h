/*************************************************************************
Copyright (c) 2020 Taoist Luo

Create by: Taoist Luo
CSDN：https://editor.csdn.net/md/?articleId=107549026

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************************************/
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