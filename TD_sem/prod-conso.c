#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include "semaph.h"

int main() {
    int segment_id;
    int* shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    const int shared_segment_size = 5 * sizeof(int);

    srand(time(NULL));

    segment_id = shmget(IPC_PRIVATE, shared_segment_size, IPC_CREAT | 0666);
    if (segment_id == -1) {
        perror("Erreur lors de la création du segment de mémoire partagée");
        exit(EXIT_FAILURE);
    }

    shared_memory = (int*)shmat(segment_id, 0, 0);
    if (shared_memory == (int*)-1) {
        perror("Erreur lors de l'attachement du segment de mémoire partagée");
        exit(EXIT_FAILURE);
    }

    shared_memory[0] = 0;
    shared_memory[1] = 0;
    shared_memory[2] = 0;
    shared_memory[3] = 0;
    shared_memory[4] = 0;

    // Initialisation des sémaphores
    int r = init_semaphore();
    if (r == -2) {
        perror("error");
    }
    r = val_sem(1, 1);
    if (r == -1) {
        perror("error");
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Erreur du fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Fils
        int iter = 0;

        while (iter < 50) {
            int n = rand() % 100;
            for (int i = 0; i < 5; i++) {
                P(1); 
                if (shared_memory[i] == 0) {
                    shared_memory[i] = n;
                    break;
                }
                V(1);

            }

            iter++;
        }

    } else {
        // Père
        while (1) {
            for (int i = 4; i >= 0; i--) {
                if (shared_memory[i] != 0) {
                    P(1); 
                    int t = shared_memory[i];
                    shared_memory[i] = 0;
                    printf("Valeur lue par le pere : %d\n", t);
                    break;
                    V(1); 

                }
            }
        }

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
