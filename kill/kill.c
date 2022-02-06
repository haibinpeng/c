#include <func.h>

//传递一个参数，argv[1]代表pid
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2)
    int ret=kill(atoi(argv[1]),SIGINT);
    ERROR_CHECK(ret,-1,"kill");
    return 0;
}