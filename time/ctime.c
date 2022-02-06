#include <func.h>

int main()
{
    //time_t now=time(NULL);//两种写法都可以
    time_t now;
    time(&now);//获取距离1970年1月1号的秒数
    printf("%ld\n", now);
    printf("字符串时间=%s",ctime(&now));//ctime转换成字符串时间，里面默认有换行
    return 0;
}