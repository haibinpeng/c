#include<func.h>

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,2)
    DIR *dir=opendir(argv[1]);
    ERROR_CHECK(dir,NULL,"opendir")
    struct dirent *p;
    struct stat buf;
    while((p=readdir(dir))!=NULL)
    {
        stat(p->d_name,&buf);
        printf("mode=%x %3ld %s %s %6ld %s %s\n",\
buf.st_mode, buf.st_nlink, getpwuid(buf.st_uid)->pw_name, getgrgid(buf.st_gid)->gr_name,buf.st_size,ctime(&buf.st_mtime)+4,p->d_name);//+4是为了去掉时间里的星期
    }
    closedir(dir);
    return 0;
}