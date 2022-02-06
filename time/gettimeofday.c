#include <func.h>

//gettimeofday获取当前时区距离1970年1月1日的秒数，微秒数
int main()
{
    struct timeval t;
    int ret=gettimeofday(&t,NULL);
    ERROR_CHECK(ret,-1,"gettimeofday")
    printf("second=%ld,micro_second=%ld\n",t.tv_sec,t.tv_usec);
    return 0;
}