#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define MAX_TASKS 10
#define BUFFER_SIZE 100

typedef struct {
    int task_id;
    char params[BUFFER_SIZE];
} Task;

int pipefd[2];

pid_t pid;

void handle_sigint(int sig, siginfo_t *info, void *context) {
    kill(pid,SIGUSR1);
}

void handle_son(int sig, siginfo_t *info, void *context) {
    exit(0);
}

void task_manager() {
    char buffer[BUFFER_SIZE];
    int task_count = 0;
    Task tasks[MAX_TASKS];

    while (1) {
        read(pipefd[0], buffer, BUFFER_SIZE);
        if (strcmp(buffer, "SIGUSR1") == 0) {
            break;
        }

        Task new_task;
        sscanf(buffer, "%d %s", &new_task.task_id, new_task.params);
        tasks[task_count++] = new_task;

        pid = fork();
        if (pid == 0) {
            printf("Executing task %d with params: %s\n", new_task.task_id, new_task.params);
            // on simule l'éxécution de la tâche pendant 5 secondes
            sleep(5);
            printf("Task %d completed\n", new_task.task_id);
            exit(0);
        } else if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
    }

    while (wait(NULL) > 0);
}




int main() {
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    struct sigaction son; 

    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = handle_sigint;
    sa.sa_flags = SA_SIGINFO;

    memset(&son, 0, sizeof(son));
    son.sa_sigaction = handle_son;
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGUSR1, &son, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == 0) {
        // dans le fils
        close(pipefd[1]); 
        task_manager();
        exit(0);
    } else if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]); 
    char task_info[BUFFER_SIZE];
    while (1) {
        printf("Veuillez entrez les taches à éxécuter (1 param1...): ");
        fgets(task_info, BUFFER_SIZE, stdin);
        write(pipefd[1], task_info, strlen(task_info));
    }

    wait(NULL);

    return 0;
}
