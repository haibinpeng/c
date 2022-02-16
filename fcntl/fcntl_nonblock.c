#include <func.h>
//修改标准输入为非阻塞

void fcntl_nonblock(int fd)
{
    int status=fcntl(fd, F_GETFL);//拿出标准输入所有属性
    //修改其为非阻塞
    status=status|O_NONBLOCK;
    fcntl(fd, F_SETFL,status);
}

int main()
{
    sleep(5);
    char buf[128]={0};
    fcntl_nonblock(STDIN_FILENO);//修改标准输入为非阻塞
    int ret=read(STDIN_FILENO, buf, sizeof(buf));
    printf("ret=%d,buf=%s\n",ret,buf);
    return 0;
}