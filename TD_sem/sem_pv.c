#include "semaph.h"


static int sem_id = -1;

union semunn{
int val ; // pour SETVAL
struct semid_ds *buf ; // pour IPC_SET et IPC_STAT
unsigned int *array ; // pour GETALL et SETALL
};

int init_semaphore(void) {
    union semunn argo;
    argo.val = 0;

    if (sem_id != -1) {
        // la fonction a déjà été appelée
        printf("fonction déjà appelée\n");
        return -1;
    }

    sem_id = semget(IPC_PRIVATE, NSEM, 0666);
    if (sem_id == -1) {
        // échec
        printf("Échec de la création\n");
        return -2;
    }

    // initialisation à 0
    for(unsigned int j=0;j<NSEM;j++){
        semctl(sem_id,j,SETVAL,argo);
    }

    return 0;
}


int detruire_semaphore(void){
    int val;
    if(sem_id == -1){
        // one ne peut pas supprimer quelque chose qui n'existe pas 
        printf("fonction pas encore appelée\n");
        return -1;
    }
    val = semctl(sem_id,0,IPC_RMID,0);
    sem_id=-1;
    return val;
}


int val_sem(int sem, int val){
    int value=0;
    if(sem_id == -1){
        // on ne peut pas supprimer quelque chose qui n'existe pas 
        printf("fonction pas encore appelée\n");
        return -1;
    }
    value = semctl(sem,0,IPC_SET,val);
    if (value == -1){
        // erreur car sémaphore n'existe pas
        return -2;
    }
    return value;
}

struct sembuf op_p = {-1,-1,0},op_v={-1,1,0};


int P(int sem){
    if(sem_id == -1){
        printf("fonction pas encore appelée\n");
        return -1;
    }
    op_p.sem_num = sem;
    int val = semop(sem,&op_p,1);
    if(val == -1){
        return -2;
    }
    return val;
}


int V(int sem){
    if(sem_id == -1){
        printf("fonction pas encore appelée\n");
        return -1;
    }
    op_v.sem_num = sem;
    int val = semop(sem,&op_v,1);
    if(val == -1){
        return -2;
    }
    return val;
}



