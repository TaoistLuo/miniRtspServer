#ifndef _H264FILE_MEDIA_SOURCE_H_
#define _H264FILE_MEDIA_SOURCE_H_
#include <string>
#include "Mutex.h"
#include "MediaSource.h"
#include "Env.h"

class H264FileMediaSource : public MediaSource
{
public:
    static H264FileMediaSource* createNew(Env* env,std::string file);

    H264FileMediaSource(Env* env,const std::string& file);
    ~H264FileMediaSource();

protected:
    virtual void readFrame();

private:
    int getFrameFromH264File(int fd, uint8_t* frame, int size);

private:
    std::string mFile;
    int mFd;
};

#endif //_H264FILE_MEDIA_SOURCE_H_