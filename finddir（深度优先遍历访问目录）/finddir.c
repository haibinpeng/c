#include<func.h>

int print_dir(char *path,int width)//width代表空格数
{
    DIR *dir=opendir(path);
    ERROR_CHECK(dir,NULL,"opendir")
    struct dirent *p;
    char buf[1024];
    while ((p=readdir(dir)) != NULL)
    {
        if (strcmp(p->d_name,".")==0||(strcmp(p->d_name,".."))==0)
        {
            continue;
        }
        printf("%*s%s\n",width,"",p->d_name);//%*s,""是打印空格的匹配方式
        if(p->d_type==4)//是目录，继续递归
        {
            memset(buf,0,sizeof(buf));//每次递归清空buf
            sprintf(buf,"%s%s%s",path,"/",p->d_name);//下一级文件夹内无法直接读取，要拼接成相对路径
            print_dir(buf,width+4);//每次递归一层，往后退四个空格
        }
    }
    closedir(dir);
}
int main(int argc, char* argv[])
{
    ARGS_CHECK(argc,2)
    print_dir(argv[1],0);
    return 0;
}