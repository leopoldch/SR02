#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#define BILLION  1000000000.0 // utile pour le temps 


// ----------------------- SEQUENTIEL -----------------------

int main() {
    //valeur max
    unsigned int n = 500000;
    //nb de thread
    unsigned int k = 1;
    //tableau
    bool tab[n + 1]; 
    // threads
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    for (unsigned int i = 2; i <= n; i++) {
        tab[i] = true;
    }

    const int sqrt_value = sqrt(n);
    for (unsigned int i = 2; i <= sqrt_value; i++) {
        if (tab[i]) {
            for (unsigned int j = i * i; j <= n; j += i) {
                tab[j] = false;
            }
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;

    // Affichage 
    printf("Temps d'execution en sequentiel pour n=%d :  %f seconds\n ",n, time_spent);
    
    /* UTILE POUR DEBUG
    printf("Nombres premiers jusqu'a %d:\n", n);
    for (unsigned int i = 2; i <= n; i++) {
        if (tab[i]) {
            printf("%d ", i);
        }
    }*/
    return 0;
}



