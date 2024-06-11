#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#define BILLION  1000000000.0 // utile pour le temps 


// ----------------------- SEQUENTIEL -----------------------
void sieve_of_eratosthenes(unsigned int n) {  // Ajout de la fonction sieve_of_eratosthenes
    // tableau
    bool tab[n + 1]; 

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

    /* UTILE POUR DEBUG
    printf("Nombres premiers jusqu'a %d:\n", n);
    for (unsigned int i = 2; i <= n; i++) {
        if (tab[i]) {
            printf("%d ", i);
        }
    }*/
}

int main() {
    // valeur max
    unsigned int n = 500000;

    // temps d'exécution
    clock_t start = clock();
    sieve_of_eratosthenes(n);  // Appel de la fonction sieve_of_eratosthenes
    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    // Affichage 
    printf("Temps d'execution en sequentiel pour n=%d :  %f seconds\n", n, time_spent);

    return 0;
}


