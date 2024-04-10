#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

int main(){
    int fd;
    char *file, *string = NULL;
    size_t size = 1024; 

    fd = open("test_exo4.txt", O_RDONLY); 
    if(fd == -1){
        perror("erreur de fd");
        return 1;
    }

    file = (char *)mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0); 
    if(file == MAP_FAILED){
        perror("Mapping Failed");
        close(fd);
        return 1;
    }
    close(fd);
    
    string = (char *)malloc(size + 1); 
    if (string == NULL) {
        perror("Erreur d'allocation de mémoire");
        munmap(file, size);
        return 1;
    }

    printf("my file : ");
    size_t j = 0;
    for(int i = size -1; i > -1 && file[i] != '\0'; i--){
        string[j++] = file[i];
    }
    string[j] = '\0'; 

    printf("\n");
    printf("file : %s \n", string);
    fflush(stdin);

    munmap(file, size); 
    free(string); 
    return 0;
}
