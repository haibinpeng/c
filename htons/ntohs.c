#include<func.h>
//把网络字节序转为主机字节序

int main()
{
    uint16_t port=0x1234,nport;
    nport=htons(port);//把主机字节序转为网络字节序
    printf("nport=%x\n",nport);
    port=ntohs(nport);
    printf("port=%x\n",port);//把网络字节序转为主机字节序
    return 0;
}