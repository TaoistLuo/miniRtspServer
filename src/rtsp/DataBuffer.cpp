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
#include "DataBuffer.h"
#include "SocketOptions.h"

#include <unistd.h>
#include "Mylog.h"
const int DataBuffer::initialSize = 1024;
const char* DataBuffer::kCRLF = "\r\n";

int DataBuffer::read(int fd)
{
    char sparebuffer[65536];
    struct iovec vec[2];
    const int writable = writableDataSize();
    vec[0].iov_base = getbufferstart()+mWriteIndex;
    vec[0].iov_len = writable;
    vec[1].iov_base = sparebuffer;
    vec[1].iov_len = sizeof(sparebuffer);
    // when there is enough space in this buffer, don't read into sparebuffer.
    // when sparebuffer is used, we read 128k-1 bytes at most.
    const int iovcnt = (writable < sizeof(sparebuffer)) ? 2 : 1;
    const int n = SocketOptions::readv(fd, vec, iovcnt);
    LOGI("receive date start*************************************************\n");
    LOGI("Read data length: %d  data:\n%s",n,vec[0].iov_base);
    LOGI("receive date end  -------------------------------------------------\n");
    if (n < 0)
    {
        return -1;
    }
    else if (n <= writable)
    {
        mWriteIndex += n;
    }
    else
    {
        mWriteIndex = mBufferSize;
        append(sparebuffer, n - writable);
    }

    return n;
}

int DataBuffer::write(int fd)
{
    return SocketOptions::write(fd, getReadStart(), readableDataSize());
}