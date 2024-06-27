#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
int val = 5; if(fork())
wait(&val); else
exit(val); val++;
printf("%d\n", val); return val; }
