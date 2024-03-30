#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#define MAX_CHILD_PROCESSES 5


int nb_task = 0;

typedef struct {
    int task_id; 
    char* params;
    struct TaskRequest* next;
} TaskRequest;


TaskRequest* createTask(char* params, unsigned int id){
    TaskRequest* newTask = malloc(sizeof(TaskRequest));
    if (newTask == NULL) {
        return NULL;
    }
    newTask->next = NULL;
    newTask->task_id = id;
    newTask->params = params; 
    return newTask; 
}

TaskRequest* deleteFirstTask(TaskRequest* file){
    TaskRequest* tmp = file->next;
    free(file);
    return tmp;
}

void addTask(TaskRequest *file, char* params, unsigned int id){
    TaskRequest* tmp = file;
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    tmp->next = createTask(params, id);
}


pid_t pid;
int verif = 1;

void handle_sigint(int sig, siginfo_t *info, void *context) {
    if(!verif){
        printf("\nLe processus gestionnaire de tâche a déjà été terminé !\n");
    }
    if(pid != 0){ 
        //printf("Éxécuté - pid fils : %d / pid courant : %d\n", pid, getpid());
        kill(pid, SIGUSR1);
        verif = 0;
    }
}

void handle_sigusr1(int sig, siginfo_t *info, void *context) {
    // on reçoit bien dans le fils et on doit maintenant kill les processus crée par le gestionnnaire de tâche ? 
    printf("Processus gestionnaire de tâche terminé.\npid : %d \n", getpid());
    exit(0);
}







int main() {
    int fd[2];
    struct sigaction pere;
    struct sigaction fils;

    fils.sa_sigaction = handle_sigusr1;
        fils.sa_flags = SA_SIGINFO;
        sigemptyset(&fils.sa_mask);

        if (sigaction(SIGUSR1, &fils, NULL) == -1) {
            perror("sigaction fils");
            exit(1);
        }
    pere.sa_sigaction = handle_sigint;
        pere.sa_flags = SA_SIGINFO;
        sigemptyset(&pere.sa_mask);
        if (sigaction(SIGINT, &pere, NULL) == -1) {
            perror("sigaction pere");
            exit(1);
        }

    // Créer une pipe pour la communication entre le processus principal et le gestionnaire de tâches
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid == 0) { //  fils (gestionaire de tâche)
        close(fd[1]); 
        int active_children = 0;

        while(1){
            char var[50]; 
            read(fd[0], var, sizeof(var));
            if(strcmp(var,"pas de tache") == 0 || strcmp(var, "") ==0){
                printf("en attente de tache \n");
            }else{
                pid_t child_pid = fork();
                if (child_pid == 0){
                    // Fils
                    printf("Execution de la tâche : %s \nPID du processus executant la tache : %d \n",var, getpid());
                    exit(0);
                } else if (child_pid > 0) {
                    // Père
                    active_children++;
                    if (active_children >= MAX_CHILD_PROCESSES) {
                        // Attendre qu'un processus fils se termine avant de continuer
                        wait(NULL);
                        active_children--;
                    }
                } else {
                    perror("fork");
                    exit(1);
                }
            }
            sleep(2);
        }

        exit(0);
    } 
    
    else if (pid > 0) { 
        // père 
        close(fd[0]); 
        
        TaskRequest* file = createTask("Task 1", 1);
        addTask(file, "Task 2", 2);
        addTask(file, "Task 3", 3);
        addTask(file, "Task 4", 4);
        addTask(file, "Task 5", 5);

        // ajouter d'autres tâches ? 
        // manière plus simple ? 

        while (1) {
            printf("Le père vit ! \n");

                if(verif){
                    if(file != NULL){
                        write(fd[1], file->params, 50);
                        file = deleteFirstTask(file);
                    }
                    else{
                    write(fd[1], "pas de tache", 50);
                    }
                }else{
                    // le gestionnaire de tâche a été tué, ne rien faire 
                }

            


            sleep(1); // Simule une pause entre les envois de demandes
        }

        
    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}
