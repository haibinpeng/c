#include<func.h>
//popen以r状态拉起进程，fp是管道的读端
int main()
{
    FILE *fp;
    fp=popen("ls -l","r");
    char buf[1024]={0};
    fread(buf,1,sizeof(buf),fp);
    printf("%s",buf);
    pclose(fp);
    return 0;
}