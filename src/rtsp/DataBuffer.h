#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <stdlib.h>
#include <algorithm>
#include <stdint.h>
#include <assert.h>



class DataBuffer
{
public:
    static const int initialSize;

    explicit DataBuffer() :
        mBufferSize(initialSize),
        mReadIndex(0),
        mWriteIndex(0)
    {
        mBuffer = (char*)malloc(mBufferSize);
    }

    ~DataBuffer()
    {
        free(mBuffer);
    }

    int readableDataSize() const
    { 
        return mWriteIndex - mReadIndex;
    }

    int writableDataSize() const
    { 
        return mBufferSize - mWriteIndex;
    }

    int getReadIndex() const
    {
        return mReadIndex;
    }
    //get start read point
    char* getReadStart()
    { 
        return getbufferstart() + mReadIndex;
    }

    const char* getReadStart() const
    { 
        return getbufferstart() + mReadIndex;
    }

    const char* lookupCRLF() const
    {
        const char* crlf = std::search(getReadStart(), getWritePoint(), kCRLF, kCRLF+2);
        return crlf == getWritePoint() ? NULL : crlf;
    }

    const char* lookupCRLF(const char* start) const
    {
        assert(getReadStart() <= start);
        assert(start <= getWritePoint());
        const char* crlf = std::search(start, getWritePoint(), kCRLF, kCRLF+2);
        return crlf == getWritePoint() ? NULL : crlf;
    }

    const char* findLastCrlf() const
    {    
        const char* crlf = std::find_end(getReadStart(), getWritePoint(), kCRLF, kCRLF+2);
        return crlf == getWritePoint() ? NULL : crlf;
    }

    void retrieve(int len)
    {
        assert(len <= readableDataSize());
        if (len < readableDataSize())
        {
            mReadIndex += len;
        }
        else
        {
            retrieveAll();
        }
    }

    void retrieveUntil(const char* end)
    {
        assert(getReadStart() <= end);
        assert(end <= getWritePoint());
        retrieve(end - getReadStart());
    }

    void retrieveAll()
    {
        mReadIndex = 0;
        mWriteIndex = 0;
    }

    char* getWritePoint()
    { 
        return getbufferstart() + mWriteIndex;
    }

    const char* getWritePoint() const
    { 
        return getbufferstart() + mWriteIndex;
    }

    void hasWritten(int len)
    {
        assert(len <= writableDataSize());
        mWriteIndex += len;
    }

    void unwrite(int len)
    {
        assert(len <= readableDataSize());
        mWriteIndex -= len;
    }

    /* make sure the buffer has enouph space */
    void ensureWritableBytes(int len)
    {
        if (writableDataSize() < len)
        {
            makeSpace(len);
        }
        assert(writableDataSize() >= len);
    }

    void makeSpace(int len)
    {
        if (writableDataSize() + getReadIndex() < len) //如果剩余空间不足
        {         
            mBufferSize = mWriteIndex+len;
            mBuffer = (char*)realloc(mBuffer, mBufferSize);
        }
        else 
        {
            int readable = readableDataSize();
            std::copy(getbufferstart()+mReadIndex,
                    getbufferstart()+mWriteIndex,
                    getbufferstart());
            mReadIndex = 0;
            mWriteIndex = mReadIndex + readable;
            assert(readable == readableDataSize());
        }
    }

    void append(const char* data, int len)
    {
        ensureWritableBytes(len);
        //copy data
        std::copy(data, data+len, getWritePoint());
        //Reset write position
        hasWritten(len);
    }

    void append(const void* data, int len)
    {
        append((const char*)(data), len);
    }

    int read(int fd);
    int write(int fd);

private:
    char* getbufferstart()
    { 
        return mBuffer;
    }

    const char* getbufferstart() const
    { return mBuffer; }

private:
    char* mBuffer;
    int mBufferSize;
    int mReadIndex;
    int mWriteIndex;

    static const char* kCRLF;
};

#endif //_BUFFER_H_