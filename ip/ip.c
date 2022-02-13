#include<func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2)
    int ret;
    struct in_addr addr;
    ret=inet_aton(argv[1],&addr);
    if(0==ret)
    {
        printf("inet_aton fail\n");
        return -1;
    }
    printf("addr=%x\n",addr.s_addr);
    printf("点分十进制=%s\n",inet_ntoa(addr));
    printf("addr=%x\n",inet_addr(argv[1]));
    return 0;
}