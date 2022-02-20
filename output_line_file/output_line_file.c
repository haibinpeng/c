#include <stdio.h>
//打印错误的文件和行号

int main()
{
    printf("error at %s %d\n",__FILE__,__LINE__);//__FILE__：正在编译文件的文件名,__LINE__：正在编译文件的行号
    return 0;
}