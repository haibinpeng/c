#include "process_pool.h"

int trans_file(int new_fd)
{
    //发送文件名
    train_t t;
    t.data_len=strlen(FILENAME);//文件名长度
    strcpy(t.buf,FILENAME);//文件名
    send(new_fd,&t,4+t.data_len,0);
    //发送文件内容
    int fd=open(FILENAME,O_RDONLY);
    ERROR_CHECK(fd,-1,"open")
    while(t.data_len=read(fd,t.buf,sizeof(t.buf)))
    {
        send(new_fd,&t,4+t.data_len,0);
    }
    t.data_len=0;
    send(new_fd,&t,4+t.data_len,0);//发送文件内容发送完毕的标志
    close(new_fd);
}