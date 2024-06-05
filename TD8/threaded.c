#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

struct ThreadData {
    int start;
    int end;
    int threadNum;
    int step;
};

struct InitData {
    int numThreads;
    pthread_t * threads;
    int size;
    struct ThreadData *data;
};

int * array;

void* threadFunction(void* threadData){
    struct ThreadData *data = threadData;
    int start;

    if(data->threadNum == 0){
        start = pow(data->start, 2);
    }
    else {
        if(data->start < pow(data->step, 2))
            start = pow(data->step, 2);
        else
            start = data->start;
    }

    for(int i = start; i <= data->end; i += data->step){
        array[i] = 0;
    }

    pthread_exit(NULL);
}

void* initialize(void * initData){
    struct InitData *data = initData;
    pthread_attr_t attr;
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_init(&attr);

    data->data = malloc(data->numThreads * sizeof(struct ThreadData));
    int end = 0;

    for(int i = 2; i < ceil(sqrt(data->size)); i++){
        if (array[i] == 1) {
            end = 0;

            for(int j = 0; j < data->numThreads; j++){
                int start = (end/i)*i+i;
                end = (j+1)*ceil((data->size)/(data->numThreads));
                data->data[j].start = start;
                data->data[j].end = end;
                data->data[j].step = i;
                data->data[j].threadNum = j;
                pthread_create(&(data->threads[j]), &attr, threadFunction, &data->data[j]);
            }

            for(int j = 0; j < data->numThreads; j++){
                    pthread_join(data->threads[j], NULL);
            }
        }
    }

    pthread_exit(NULL);
}

double main(){
    int size;
    int numThreads;
    pthread_t *threads;
    pthread_t mainThread;
    double startTime;
    double endTime;
    double totalTime;

    pthread_attr_t attr;
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_init(&attr);

    printf("Enter the size for the calculation: ");
    scanf("%d", &size);

    printf("Enter the number of threads to use: ");
    scanf("%d", &numThreads);

    startTime = clock();

    array = malloc(sizeof(int)*(size+1));
    threads = malloc(sizeof(pthread_t)*numThreads);

    for(int i = 0; i < size; i++){
        array[i] = 1;
    }

    struct InitData initData = {numThreads, threads, size+1, NULL};
    int result = pthread_create(&mainThread, &attr, initialize, &initData);
    if(result != 0){
        perror("Thread creation error");
        exit(1);
    }
    pthread_join(mainThread, NULL);

    for(int i = 0; i < size; i++){
        printf("array[%d] = %d \n", i, array[i]);
    }

    endTime = clock();
    totalTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    printf("Temps d'éxécution : %f \n", totalTime);

    return totalTime;
}
