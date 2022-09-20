#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main()
{
    char str[20];
    int val, num;
    printf("Please enter some words: ");
    val = scanf("%s %d", str, &num);
    printf("The sentence is %s\nThe return value is %d\n", str, val);

    return 0;
}