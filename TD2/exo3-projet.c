#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t task_manager_pid;

typedef struct Task{

    unsigned int id_tache;
    char *parameters;

}Task;

struct sigaction exit_manager;
struct sigaction task_manager;

void task_controller(int signum){
    fflush(stdout);
    printf("Signal reçu depuis le père\n");
    exit(0);
}

void exit_controller(int signum){
    fflush(stdout);
    printf("ctrl-c reçu\n le pid : %d", task_manager_pid);
    // appel au fils

    kill(task_manager_pid, SIGUSR1);
}

int main(){
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    task_manager_pid = fork();
    if(task_manager_pid < 0){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(task_manager_pid == 0){
        // dans le fils "gestionnaire de tâches"
        task_manager.sa_handler = task_controller;
        sigemptyset(&task_manager.sa_mask);
        task_manager.sa_flags = 0;

        if (sigaction(SIGUSR1, &task_manager, NULL) == -1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }

        while(1){
            sleep(1);
            fflush(stdout);
            printf("je suis là\n");
        }

    }else{

        // dans le père aka main 


        exit_manager.sa_handler = exit_controller;
        sigemptyset(&exit_manager.sa_mask);
        exit_manager.sa_flags = 0;

        if (sigaction(SIGINT, &exit_manager, NULL) == -1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
        sleep(1); 
        // ici on doit pouvoir envoyer des tâches à traiter au fils 

        Task t1 = {1, "coucou"};



    }
    return 0;
}
