#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    long l;
    char *str = "4294967295";
    l = atol(str);
    printf("string = %s integer = %ld\n",str,l);
    return 0;
}
