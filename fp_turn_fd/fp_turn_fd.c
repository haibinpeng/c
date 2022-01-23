#include<func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2)
    FILE *fp=fopen(argv[1],"rb+");
    ERROR_CHECK(fp,NULL,"fopen");
    int fd=fileno(fp);//fd转fp：FILE *fp=fdopen(fd,"rb+");
    printf("%d\n",fd);
    char buf[128]={0};
    read(fd,buf,sizeof(buf));
    printf("%s\n",buf);
    fclose(fp);
    return 0;
}