#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#define MAX_CHILD_PROCESSES 5
#define TIME_OUT_PROCESS 2


int nb_task = 0;

//Definition de la structure de tache : Numero de tache, paramètre de tache et tache suivante.
typedef struct {
    int task_id; 
    char* params;
    struct TaskRequest* next;
} TaskRequest;


//Creation d'un tache ; renvoie pointeur
TaskRequest* createTask(char* params, unsigned int id){

    TaskRequest* newTask = malloc(sizeof(TaskRequest));
    if (newTask == NULL) {
        return NULL;
    }
    newTask->next = NULL;
    newTask->task_id = id;
    newTask->params = malloc(strlen(params)+1);
    strcpy(newTask->params, params);
    return newTask; 
}

//Prend la premiere tache la retire et l'envoie
TaskRequest* deleteFirstTask(TaskRequest* file){
    TaskRequest* tmp = file->next;
    free(file);
    return tmp;
}

//Ajoute une tache a la fin
void addTask(TaskRequest *file, char* params, unsigned int id){
    TaskRequest* tmp = file;
    while(tmp->next != NULL){
        tmp = tmp->next;
    }
    tmp->next = createTask(params, id);
}


pid_t pid; //PID Gestionnaire
pid_t child_pid; //PID des sous fils du gestionnaire (execution de tache)
int verif = 1;

//Attend le signal CTR C et arrete le gestionnaire de taches
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

//Attend le signal SIGUSR1 et arrete le processus courant (sur le gestionnnaire de tache)
void handle_sigusr1(int sig, siginfo_t *info, void *context) {
    // on reçoit bien dans le fils et on doit maintenant kill les processus crée par le gestionnnaire de tâche ? 
    printf("Processus gestionnaire de tâche terminé.\npid : %d \n", getpid());
    exit(0);
}

//Attend un signal alarm pour arreter les sous fils du gestionnaire de taches (Execution en cours)
void handle_alarm(int sig, siginfo_t *info, void *context) {
    if (kill(child_pid, 0) == 0) {
        printf("\tProcessus %d time out\n", child_pid);
        kill(child_pid, SIGTERM);
    }
}

int main() {
    int fd[2];
    struct sigaction pere;
    struct sigaction fils;
    struct sigaction fils_alarm;

    //Gestionnaire de signal pour arret
    fils.sa_sigaction = handle_sigusr1;
        fils.sa_flags = SA_SIGINFO;
        sigemptyset(&fils.sa_mask);

        if (sigaction(SIGUSR1, &fils, NULL) == -1) {
            perror("sigaction fils");
            exit(1);
        }
    //Gestionnaire de signal pour arret CTRC
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
                //printf("en attente de tache \n");
            }else{

                child_pid = fork();
                if (child_pid == 0){
                    // Fils
                    printf("Execution de la tâche : %s \nPID du processus executant la tache %d :\n",var, getpid());
                    system(var);
                    exit(0);
                } else if (child_pid > 0) {

                    int status;
                    
                    //gestionnaire de signal pour attendre l'alarme
                    fils_alarm.sa_sigaction = handle_alarm;
                    fils_alarm.sa_flags = SA_SIGINFO;
                    sigemptyset(&fils_alarm.sa_mask);
                    if (sigaction(SIGALRM, &fils_alarm, NULL) == -1) {
                        perror("sigaction fils alarm");
                        exit(1);
                    }

                    alarm(TIME_OUT_PROCESS);
                    waitpid(child_pid, &status, 0);

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
        
        TaskRequest* file = createTask("pwd", 1);
        //addTask(file, "ps -ef", 2);

        char task[50] = "";
        char temp[50];
        int compteur = 2;

        while(1) {
            memset(temp, 0, sizeof(char)*50);
            memset(task, 0, sizeof(char)*50);

            printf("Rentrez une commande : ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0;
            strcpy(task, temp);

            if(strcmp(task, "sendtasks") != 0) {
                addTask(file, task, compteur);
                compteur++;
                printf("Commande choisie : %s\n", task);
            } else {
                break;
            }

        }

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
