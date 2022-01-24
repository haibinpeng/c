#include <func.h>

int main()
{
    printf("uid:%d,gpid:%d,euid:%d,egid:%d\n",getuid(),getgid(),geteuid(),getegid());
    int fd;
    fd=open("file",O_RDWR);
    ERROR_CHECK(fd,-1,"open")
    write(fd,"I can write",11);//加权限：chmod u+s modify_file
    close(fd);
    return 0;
}