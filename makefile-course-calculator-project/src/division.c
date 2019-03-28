#include <stdlib.h>
#include <stdio.h>

double division(float x, float y)
{
    if(y == 0)
    {
        printf("Divisão por Zero!\n");
        exit(0);
    }
    else
        return x / y;
}