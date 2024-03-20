//
// Created by Léopold Chappuis on 20/03/2024.
//
#include "stdlib.h"
#include "stdio.h"
#include "signal.h"
#include "unistd.h"
#include "time.h"
#include <sys/random.h>

int task_manager_pid;

struct sigaction exit_manager;
struct sigaction task_manager;

void task_controller(){
    fflush(stdout);
    printf("Signal reçu depuis le père");
    exit(0);
}

void exit_controller(){
    // appel au fils
    fflush(stdout);
    printf("ctrl-c reçu");
    kill(task_manager_pid, SIGUSR1);
}

int main(){
    int fd[2];
    pipe(fd);

    task_manager_pid = fork();
    if(task_manager_pid <0){
        printf("Erreur lors du fork");
    }
    else if(task_manager_pid==0){
        // dans le fils "gestinnaire de tache")
        task_manager.sa_handler = task_controller;
        sigaction(SIGUSR1, &task_manager, 0);
        while(1){
            sleep(1);
            fflush(stdout);
            printf("je suis là\n");
        }
    }else{
        // dans le père
        exit_manager.sa_handler = exit_controller;
        sigaction(SIGINT, &exit_manager, NULL);
        sigemptyset(&exit_manager.sa_mask);
        sleep(1);
        while (1){
            pause();
        }
    }
    return 0;
}


