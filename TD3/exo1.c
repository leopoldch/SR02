//
// Created by Léopold Chappuis on 20/03/2024.
//
#include "stdlib.h"
#include "stdio.h"
#include "signal.h"
#include "unistd.h"
#include "time.h"
#include <sys/random.h>

struct  sigaction act_fils;
struct  sigaction act_pere;
int pid;

void handlesigalarm(){
    // faire un appel à un signal pour
    kill(pid, SIGUSR1);
    alarm(5);
}

void handlesigusr(){
    // tirer un chiffre aléatoire et l'afficher
    srand(time(NULL));
    int randomNumber = (rand() % 31 )+10;
    printf("\n Température : %d\n", randomNumber);
}

int main(){

    pid = fork();
    if(pid <0){
        printf("Erreur lors du fork");
    }
    else if(pid==0){
        act_fils.sa_handler = handlesigusr;
        sigaction(SIGUSR1, &act_fils, 0);
        while(1){
            pause();
        }
    }else{
        act_pere.sa_handler = handlesigalarm;
        sigaction(SIGALRM, &act_pere,0);
        alarm(5);
        while(1){
            sleep(1);
            printf("-");
            fflush(stdout);
        }
    }
    return 0;
}


