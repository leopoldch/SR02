#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_THREADS 4

/*
Explication du code
1. **Optimisation de la boucle interne** :
   - La boucle interne est modifiée pour utiliser un pas de `2i` au lieu de `i`. Cela permet de sauter les nombres pairs.

2. **Réduction de l’espace mémoire** :
   - Le tableau `isPrime` stocke uniquement les valeurs pour les nombres impairs, réduisant ainsi la taille du tableau de moitié.

3. **Structure du code** :
   - La fonction `mark_multiples` est ajustée pour utiliser le pas de `2i`.
   - La fonction `sieve_of_eratosthenes_parallel` gère la création et la synchronisation des threads.
   - La fonction `measure_performance` mesure les performances pour différentes valeurs de `n`.
*/

typedef struct {
    int start;
    int end;
    bool *isPrime;
    int prime;
} thread_data_t;

void *mark_multiples(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    for (int j = data->start; j <= data->end; j += data->prime * 2) { // Optimisation : pas de 2i
        data->isPrime[j] = false;
    }
    pthread_exit(NULL);
}

void sieve_of_eratosthenes_parallel(int n, int num_threads) {
    int size = (n + 1) / 2;
    bool *isPrime = malloc(size * sizeof(bool));
    for (int i = 0; i < size; i++) isPrime[i] = true;

    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];

    for (int i = 3; i <= sqrt(n); i += 2) {
        if (isPrime[i / 2]) {
            int range = (n / 2 - i * i / 2) / num_threads;
            for (int t = 0; t < num_threads; t++) {
                thread_data[t].start = i * i / 2 + t * range;
                thread_data[t].end = (t == num_threads - 1) ? n / 2 : i * i / 2 + (t + 1) * range - 1;
                thread_data[t].isPrime = isPrime;
                thread_data[t].prime = i;
                pthread_create(&threads[t], NULL, mark_multiples, (void *)&thread_data[t]);
            }
            for (int t = 0; t < num_threads; t++) {
                pthread_join(threads[t], NULL);
            }
        }
    }

    printf("2 ");
    for (int i = 1; i < size; i++) {
        if (isPrime[i]) printf("%d ", 2 * i + 1);
    }
    printf("\n");

    free(isPrime);
}

void measure_performance(int n) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    sieve_of_eratosthenes_parallel(n, NUM_THREADS);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Parallel Time with %d threads: %f\n", NUM_THREADS, cpu_time_used);
}

int main() {
    int ns[] = {500000, 1000000, 2000000, 4000000};
    for (int i = 0; i < 4; i++) {
        printf("Measuring performance for n = %d\n", ns[i]);
        measure_performance(ns[i]);
    }
    return 0;
}