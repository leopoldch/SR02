#include "semaphore.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"



int main(){

    pid_t pid = fork();
    if(pid == -1){
        perror("erreur fork");
        exit(0);
    }
    if(pid == 0){
        // fils

        
    }else{
        //pere 


    }




    return 0;
}