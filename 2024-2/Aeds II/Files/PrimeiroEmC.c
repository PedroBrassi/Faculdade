#include <stdio.h>

int main()
{
    int num1 = 10;
    int *pont1 = &num1;

    printf("%d\n", num1);
    printf("%p\n", &num1);
    printf("--------\n");

    printf ("%p\n", pont1);
    printf ("%d\n", pont1);
    printf("--------\n");

    printf ("%p\n", &pont1);
    printf ("%d\n", *pont1);
    printf("--------\n");


}