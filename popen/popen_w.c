#include<func.h>
//popen以w状态拉起进程，fp是管道的写端
//OJ的测试原理
int main()
{
    FILE *fp;
    fp=popen("./sub","w");
    char buf[1024]="10 5";
    fwrite(buf,1,strlen(buf),fp);
    pclose(fp);
    return 0;
}