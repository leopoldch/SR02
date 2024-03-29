#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

typedef struct {
    int task_id; 
    char* params;
    struct TaskRequest* next;
} TaskRequest;


pid_t pid;
int verif = 1;

void handle_sigint(int sig, siginfo_t *info, void *context) {
    if(!verif){
        printf("\nLe processus gestionnaire de tâche a déjà été terminé !\n");
    }
    if(pid != 0){ // on doit l'éxécuter depuis le père et l'envoyer au fils 
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

void execute_task(TaskRequest *request) {
    printf("Exécution de la tâche %d\n", request->task_id);
    sleep(1); 
}

void task_manager(int read_fd) {

    TaskRequest* main = malloc(sizeof(TaskRequest));
    main->next = NULL;
    main->params = NULL;
    main->task_id = 0;

    TaskRequest request;
    pid_t pid;
    int status;

    while (1) {
        // Lire la demande de tâche de la file d'attente
        if (read(read_fd, &request, sizeof(TaskRequest)) == sizeof(TaskRequest)) {
            // Créer un processus enfant pour exécuter la tâche
            pid = fork();
            if (pid == 0) { // Processus enfant
                execute_task(&request);
                exit(0);
            } else if (pid > 0) { // Processus parent (gestionnaire de tâches)
                // Attendre que le processus enfant termine
                waitpid(pid, &status, 0);
                // Envoyer un message de retour au client de tâches
                // Ici, vous pouvez ajouter le code pour envoyer le message de retour
            } else {
                perror("fork");
                exit(1);
            }
        } else {
            perror("erreur de lecture");
            break;
        }
    }
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
        
        //task_manager(fd[0]); // Exécuter le gestionnaire de tâches
        char* myvar;
        read(fd[0], myvar, 5);
        printf("\n myvar = %s", myvar);
        while(1){
            printf("Le fils vit !\n");
            sleep(1);
        }

        exit(0);
    } 
    
    else if (pid > 0) { 
        // père 

        close(fd[0]); 
        
        while (1) {
            printf("Le père vit ! \n");

            // Ici, vous pouvez ajouter le code pour envoyer des demandes de tâches
            // via le pipe, par exemple en utilisant write(fd[1], &request, sizeof(TaskRequest));

            write(fd[1], "test", 5);

            sleep(1); // Simule une pause entre les envois de demandes
        }

        
    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}
