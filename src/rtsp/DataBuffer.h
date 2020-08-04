/*************************************************************************
Copyright (c) 2020 Taoist Luo

Create by: Taoist Luo
CSDN：https://blog.csdn.net/daichanzhang9734/article/details/107549026

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