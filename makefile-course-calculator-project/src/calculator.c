#include <stdio.h>
#include <stdio_ext.h>
#include <ctype.h>
#include "operators.h"

void drawHeader()
{
    printf("******************\n");
    printf("*  CALCULATOR  *\n");
    printf("******************\n");    
}

void menu(int *option)
{
    printf("*************************\n");
    printf("*  Choose an operation  *\n");
    printf("*                    *\n");
    printf("* [+] Addition       *\n");
    printf("* [-] Subtraction    *\n");
    printf("* [*] Multiply       *\n");
    printf("* [/] Division       *\n");
    printf("* [e] Exit           *\n");
    printf("*                    *\n");
    printf("*************************\n\n");
    printf("Operation: ");
    __fpurge(stdin);
    *option = getchar();
}

int main()
{
    float x, y;
    double result;
    char option, EXIT='e';
    
    drawHeader();

    do
    {
        menu(&option);
        if(option != EXIT)
        {
            printf("Please, insert first number: ");
            scanf("%f", &x);
            __fpurge(stdin);
            printf("Please, insert second number: ");
            scanf("%f", &y);
            __fpurge(stdin);
            
            switch(option)
            {
                case SUM:
                    result = sum(x, y);
                    break;
                case MINUS:
                    result = minus(x, y);
                    break;
                case MULTIPLY:
                    result = multiply(x, y);
                    break;
                case DIVISION:
                    result = division(x, y);
                    break;
                default:
                    printf("Invalid Option!\n"); 
            }
            printf("Result is: %lf\n\n", result);
            printf("Do you want continue?[y/n]\n");
            __fpurge(stdin);
            option = getchar();
            if(tolower(option) == 'n')
                option = EXIT;  
        }
    } while(option != EXIT);
}
