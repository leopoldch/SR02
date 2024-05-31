#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#define N 2

int main() {
    int segment_id;
    int* shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    const int shared_segment_size = sizeof(int);

    srand(time(NULL));

    segment_id = shmget(IPC_PRIVATE, shared_segment_size, IPC_CREAT | 0666);
    if (segment_id == -1) {
        perror("Erreur lors de la création du segment de mémoire partagée");
        exit(EXIT_FAILURE);
    }

    shared_memory = (int*) shmat(segment_id, 0, 0);
    if (shared_memory == (int*) -1) {
        perror("Erreur lors de l'attachement du segment de mémoire partagée");
        exit(EXIT_FAILURE);
    }

    *shared_memory = 0;


    // initialisation des sémaphores : 

    int r = init_semaphore();
    if(r == -2){
        perror("error");
    }
    r = val_sem(N,1);
    if (r==-1){
        perror("error");
    }




    pid_t pid = fork();
    if (pid == -1) {
        perror("Erreur du fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Fils
        int A = 0;
        P(N);
        for (unsigned int i = 0; i < 100; i++) {
            int n = rand() % 200 + 20;
            usleep(n);
            A++;
            *shared_memory = A;
            n = rand() % 200 + 20;
            usleep(n);
        }
        V(N);
    } else {
        // Père
        int A = 0;
        P(N);
        for (unsigned int i = 0; i < 100; i++) {
            int n = rand() % 200 + 20;
            usleep(n);
            A++;
            *shared_memory = A;
            n = rand() % 200 + 20;
            usleep(n);
            printf("Valeur de E : %d\n", *shared_memory);
        }
        V(N);

        if (shmdt(shared_memory) == -1) {
            perror("Erreur lors du détachement du segment de mémoire partagée");
            exit(EXIT_FAILURE);
        }

        if (shmctl(segment_id, IPC_RMID, NULL) == -1) {
            perror("Erreur lors de la suppression du segment de mémoire partagée");
            exit(EXIT_FAILURE);
        }
    }

    detruire_semaphore();
    return 0;
}
