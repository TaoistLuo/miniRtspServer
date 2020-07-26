#include "RtspServer.h"
#include "Mylog.h"

RtspServer* RtspServer::createNew(Env* env, Ipv4Address& addr)
{
    return new RtspServer(env,addr);
}

RtspServer::RtspServer(Env* env,const Ipv4Address& addr):TcpServer(env,addr)
{
    mTriggerEvent = TriggerEvent::createNew(this);
    mTriggerEvent->setTriggerCallback(triggerCallback);
}

RtspServer::~RtspServer(){

}
bool RtspServer::addMeidaSession(MediaSession* mediaSession)
{
    if(mMediaSessions.find(mediaSession->name()) != mMediaSessions.end()){
        return false;
    }
    mMediaSessions.insert(std::make_pair(mediaSession->name(), mediaSession));
    return true;
}

void RtspServer::handleNewConnection(ClientInfo* clientinfo){
    LOGI("New client connet\n");
    LOGI("client IP:　 %s \n",clientinfo->clientIp);
    LOGI("client port: %d \n",clientinfo->clientPort);
    LOGI("client fd :  %d \n",clientinfo->clientSockfd);
    RtspConnection* conn = RtspConnection::createNew(this, clientinfo->clientSockfd);
    conn->setDisconnectionCallback(disconnectionCallback, this);
    mConnections.insert(std::make_pair(clientinfo->clientSockfd, conn));
    return;
}
void RtspServer::disconnectionCallback(void* arg, int sockfd){
    LOGI("A call disconnect \n");
}

MediaSession* RtspServer::loopupMediaSession(std::string name)
{
    std::map<std::string, MediaSession*>::iterator it = mMediaSessions.find(name);
    if(it == mMediaSessions.end()){
        return NULL;
    }
    return it->second;
}
void RtspServer::triggerCallback(void* arg)
{
    RtspServer* rtspServer = (RtspServer*)arg;
    rtspServer->handleDisconnectionList();
}

std::string RtspServer::getUrl(MediaSession* session)
{
    char url[200];

    snprintf(url, sizeof(url), "rtsp://%s:%d/%s", SocketOptions::getLocalIp().c_str(),
                mAddr.getPort(), session->name().c_str());

    return std::string(url);
}

void RtspServer::handleDisconnectionList()
{
 //上所
    for(std::vector<int>::iterator it = mDisconnectionlist.begin(); it != mDisconnectionlist.end(); ++it)
    {
        int sockfd = *it;
        std::map<int, RtspConnection*>::iterator _it = mConnections.find(sockfd);
        assert(_it != mConnections.end());
        delete _it->second;
        mConnections.erase(sockfd);
    }
    mDisconnectionlist.clear();
}