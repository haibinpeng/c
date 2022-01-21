#include<func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2)
    DIR *dir = opendir(argv[1]);
    ERROR_CHECK(dir,NULL,"opendir")
    struct dirent *p;
    off_t pos;
    while ((p=readdir(dir)) != NULL)
    {
        printf("ino=%ld len=%d type=%d name=%s\n",p->d_ino,p->d_reclen,p->d_type,p->d_name);
        if(strcmp(p->d_name,"seekdir")==0)
        {
            pos=telldir(dir);
        }
    }
    seekdir(dir,pos);
    p=readdir(dir);
    printf("ino=%ld len=%d type=%d name=%s\n",p->d_ino,p->d_reclen,p->d_type,p->d_name);
    return 0;
}