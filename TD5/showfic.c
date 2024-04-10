#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(){
    int fd, *ptr, nb = -1;
    size_t size = 1024; 

    // Ouverture du fichier en mode lecture/écriture, création si nécessaire
    fd = open("titi.dat", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(fd == -1){
        perror("erreur de fd");
        return 1;
    }

    //  le fichier a la taille nécessaire
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
            munmap(ptr, size); // Détacher le mappage avant de terminer
            exit(1);
        }
        else
        {
            for (size_t i = 0; i < 10; i++)
            {
                printf("valeur stockées : %d \n", ptr[i]);
            }
        }
    }
    
    munmap(ptr, size); // Détacher le mappage avant de terminer
    return 0;
}
