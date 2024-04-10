#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(){
    int fd, *ptr, i = 0, nb = -1;
    size_t size = 1024; 

    fd = open("titi.dat", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(fd == -1){
        perror("erreur de fd");
        return 1;
    }

    if (ftruncate(fd, size) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }
    
    ptr = (int *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED){
        perror("Mapping Failed");
        close(fd);
        return 1;
    }
    close(fd);

    while (1)
    {   
        printf("Veuillez écrire un nombre : ");
        scanf("%d",&nb);
        if(nb == 99) {
            munmap(ptr, size); 
            exit(1);
        }
        else{
            ptr[i]++;
            if(i ==9) i = 0; 
            else i ++;
        }
    }
    
    munmap(ptr, size); 
    return 0;
}
