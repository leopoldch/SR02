#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>




void philosophe(const int numero){
    // doit prendre la fourchète i et la fouchète i+1


}


int main(){

    pid_t pid = fork();
    if(pid<0){
        printf("Error");
        return 1;
    }
    if(pid == 0){
        // fils

        // on initialise le tableau de sémaphore 
        sem_t tab[5];
        for(int i; i<5;i++){
            sem_init(&tab[i], 0, i+1);
        }



    }else{
        //père 

    }


    return 0;
}