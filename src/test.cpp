#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *i = (int *) malloc(50);

    printf("%ld\n", sizeof(i));

    return 0;
}