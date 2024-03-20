//
// Created by Léopold Chappuis on 20/03/2024.
//
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

int nb = 1;

int pos_fils = 97;
int pos_fils_fin = 122;
int pos_pere = 65;
int pos_fin_pere = 90;


void handlesigusr_pere(){
    if(pos_pere >= pos_fin_pere+2){
        return;
    }
    else{
        fflush(stdout);
        int temp = pos_pere;
        pos_pere = pos_pere + nb;
        for (int i = temp; i < pos_pere; ++i) {
            printf("%c", i);
        }
        if(nb<6){
            nb++;
        }
        else if(nb == 6){
            nb--;
        }

        kill(pid, SIGUSR1);
    }
}

void handlesigusr_fils(){
    if(pos_fils >= pos_fils_fin+2){
        return;
    }
    else{
        fflush(stdout);
        int temp = pos_fils;
        pos_fils = pos_fils + nb;
        for (int i = temp; i < pos_fils; ++i) {
            printf("%c", i);
        }
        if(nb<6){
            nb++;
        }
        else if(nb == 6){
            nb--;
        }
        kill(getppid(), SIGUSR2);
    }
}

int main(){

    pid = fork();
    if(pid <0){
        printf("Erreur lors du fork");
    }
    else if(pid==0){
        act_fils.sa_handler = handlesigusr_fils;
        sigaction(SIGUSR1, &act_fils, 0);
        while (1){
            pause();
        }
        exit(0);
    }else{
        act_pere.sa_handler = handlesigusr_pere;
        sigaction(SIGUSR2, &act_pere,0);
        sleep(1);
        kill(pid, SIGUSR1);

        while (1){
            pause();
        }

    }
    return 0;
}


