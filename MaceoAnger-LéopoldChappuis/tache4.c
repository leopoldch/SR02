#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

float compute_sequential(int size) {
    clock_t start_time, end_time;
    int *array = malloc(size * sizeof(int));
    start_time = clock();

    for (int i = 0; i < size; i++) {
        array[i] = 1;
    }

    for (int i = 2; i <= sqrt(size); i++) {
        if (array[i] == 1) {
            for (int j = i * i; j < size; j += i) {
                array[j] = 0;
            }
        }
    }

    end_time = clock();
    free(array);

    return ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
}

typedef struct {
    int start;
    int end;
    int thread_num;
    int step;
} task_data;

typedef struct {
    int num_threads;
    pthread_t *thread_ids;
    int size;
    task_data *tasks;
} thread_params;

int *shared_array;

void* execute_task(void* arg) {
    task_data *data = (task_data*)arg;
    int start_index = (data->thread_num == 0) ? data->start * data->start : data->start;
    for (int j = start_index; j <= data->end; j += data->step) {
        shared_array[j] = 0;
    }
    pthread_exit(NULL);
}

void* initialize_threads(void* arg) {
    thread_params *params = (thread_params*)arg;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    params->tasks = malloc(params->num_threads * sizeof(task_data));

    for (int i = 2; i < sqrt(params->size); i++) {
        if (shared_array[i] == 1) {
            for (int t = 0; t < params->num_threads; t++) {
                int start = (t * params->size) / params->num_threads;
                int end = ((t + 1) * params->size) / params->num_threads;
                params->tasks[t].start = start;
                params->tasks[t].end = end;
                params->tasks[t].step = i;
                params->tasks[t].thread_num = t;
                pthread_create(&params->thread_ids[t], &attr, execute_task, &params->tasks[t]);
            }

            for (int t = 0; t < params->num_threads; t++) {
                pthread_join(params->thread_ids[t], NULL);
            }
        }
    }
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}

float compute_parallel(int size, int num_threads) {
    pthread_t main_thread;
    thread_params params;
    clock_t start_time, end_time;

    shared_array = malloc((size + 1) * sizeof(int));
    params.thread_ids = malloc(num_threads * sizeof(pthread_t));
    params.num_threads = num_threads;
    params.size = size + 1;

    for (int i = 0; i < size; i++) {
        shared_array[i] = 1;
    }

    start_time = clock();
    pthread_create(&main_thread, NULL, initialize_threads, &params);
    pthread_join(main_thread, NULL);
    end_time = clock();

    free(shared_array);
    free(params.thread_ids);
    free(params.tasks);

    return ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
}

int main() {
    int sizes[] = {500000, 1000000, 2000000, 4000000};
    int p = 10; // Nombre de répétitions des algorithmes

    for (int s = 0; s < 4; s++) {
        int n = sizes[s];
        printf("n = %d : \n", n);

        double sequential_time_avg = 0;
        for (int i = 0; i < p; i++) {
            sequential_time_avg += compute_sequential(n);
        }
        sequential_time_avg /= p;

        double parallel_time_avg[7] = {0};

        for (int k = 1; k <= 7; k++) {
            for (int i = 0; i < p; i++) {
                parallel_time_avg[k-1] += compute_parallel(n, k);
            }
            parallel_time_avg[k-1] /= p;
        }

        printf("Average results (%d runs): \nseq=%f\n", p, sequential_time_avg);
        for (int k = 1; k <= 7; k++) {
            printf("par%d=%f\n", k, parallel_time_avg[k-1]);
        }
        printf("------------------------------------------------------------------------\n");
    }

    return 0;
}