#include "sharemem.h"
#define t_max 1024


int copie_fichier(int f1, int f2) { //f1 et f2 descripteur
  char buf[t_max];
  int nb_lu = 0;
  int nb_ecrit = 0;
  int nb_total = 0;

  for ( ; ; ) {
    if ((nb_lu = read(f1, buf, t_max)) <= 0) {
      break;
    }
    if ((nb_ecrit = write(f2, buf, nb_lu)) == -1) {
      break;
    }
    nb_total = nb_total + nb_ecrit;
  }

  return nb_total;
}

pid_t pid;

int main(int argc, char const *argv[]) {

  if(argc < 3) {
    printf("Argument(s) manquant(s)\n");
  } else {
    printf("ARG 0 : %s\n", argv[0]);
    printf("ARG 1 : %s\n", argv[1]);
    printf("ARG 2 : %s\n", argv[2]);
  }

  int file1 = open(argv[1], O_RDONLY);
  int file2 = open(argv[2], O_RDONLY);
  int nb1, nb2;

  int shmid;
  key_t key;
  int *shm;
  key=5678;

  if ((shmid = shmget(key, t_max, IPC_CREAT | 0666)) < 0) {
    perror("shmget");
    exit(1);
  }

  if ((shm = shmat(shmid, NULL, 0)) == (int *)-1) {
    perror("shmat");
    exit(1);
  }

  pid = fork();
  if (pid < 0) {
    perror("Fork\n");
    exit(1);
  } else if (pid == 0) { // fils
    nb1 = copie_fichier(file1, 1) -1;

    *shm = nb1;
  } else { //pere

    wait(NULL);

    nb1 = copie_fichier(file2, 1) -1;
    nb2 = *shm;
    printf("NB LU par fils : %d\n", nb2);
    printf("NB LU par parent : %d\n", nb1);
    printf("NB LU TOTAL : %d\n", nb1+nb2);


  }

  return 0;
}
