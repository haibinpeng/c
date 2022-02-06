#include<func.h>

int main()
{
    if(!fork())
    {
        while(1);
        return 0;
    }else{
         while(1);
         return 0;
    }
}