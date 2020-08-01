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
#include "Ipv4Address.h"

Ipv4Address::Ipv4Address()
{
    mPort = 8554;
    mAddr.sin_family = AF_INET;		  
    mAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    mAddr.sin_port = htons(8554);
}
Ipv4Address::Ipv4Address(uint16_t port)
{
    mPort = port;
    mAddr.sin_family = AF_INET;		  
    mAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    mAddr.sin_port = htons(port);
}

Ipv4Address::Ipv4Address(std::string ip, uint16_t port) :
    mIp(ip),
    mPort(port)
{
    mAddr.sin_family = AF_INET;		  
    mAddr.sin_addr.s_addr = inet_addr(ip.c_str()); 
    mAddr.sin_port = htons(port);
}

void Ipv4Address::setAddr(std::string ip, uint16_t port)
{
    mIp = ip;
    mPort = port;
    mAddr.sin_family = AF_INET;		  
    mAddr.sin_addr.s_addr = inet_addr(ip.c_str()); 
    mAddr.sin_port = htons(port);
}

std::string Ipv4Address::getIp()
{
    return mIp;
}

uint16_t Ipv4Address::getPort()
{
    return mPort;
}

struct sockaddr* Ipv4Address::getAddr()
{
    return (struct sockaddr*)&mAddr;
}
