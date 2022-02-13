#include<func.h>
//把主机字节序转为网络字节序

int main()
{
    uint16_t port=0x1234,nport;
    nport=htons(port);//把主机字节序转为网络字节序
    printf("nport=%x\n",nport);
    return 0;
}