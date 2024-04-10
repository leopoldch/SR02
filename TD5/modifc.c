#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(){
    int tab2[10];
    int fd, *ptr;


    fd = open("titi.dat", O_RDWR);
    if(fd == -1){
        perror("erreur de fd");
    }
    ptr = (int *)mmap(NULL,1024,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr == MAP_FAILED){
        printf("Mapping Failed\n");
        return 1;
    }
    close(fd);


    int i = 0;
    int nb = -1;
    while (nb != 99)
    {   

        printf("Veuillez écrire un nombre : ");
        scanf("%d",&nb);
        ptr[i]++;
        if(i==9){
            i = 0;
        }else{
            i++;
        }
    }
    
    for (size_t i = 0; i < 10; i++)
    {
        printf("valeur stockées : %d \n", ptr[i]);
    }
    

    return 0;
}


