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