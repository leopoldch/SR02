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
  pid = fork();
  if (pid < 0) {
    perror("Fork\n");
    exit(1);
  } else if (pid == 0) { // fils
    nb1 = copie_fichier(file1, 1) -1;
    printf("NB LU : %d\n", nb1);
  } else { //pere
    nb2 = copie_fichier(file2, 1) -1;
    printf("NB LU : %d\n", nb2);

  }

  return 0;
}
