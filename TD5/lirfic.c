#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int tab1[10] = {11,22,33,44,55,66,77,88,99,100};
    int tab2[10];
    int i,fd;
    fd = open("titi.dat", O_RDWR|O_CREAT|O_TRUNC,0666);
    read(fd, tab2, 10*sizeof(int));
    close(fd);

    for (size_t i = 0; i < 10; i++)
    {
        printf("valeur du tableau : %d \n", tab1[i]);
    }
    fflush(stdin);
}