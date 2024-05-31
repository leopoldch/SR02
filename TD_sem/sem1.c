#include <stdio.h>
#include "semaph.h"
#include <unistd.h> 

int main(){

    init_semaphore();
    val_sem(2,1);
    P(2);
    sleep(30);
    V(2);
    detruire_semaphore();

    return 0;
}