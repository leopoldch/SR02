#include "sharemem.h"

#define t_max 1024


int copie_fichier(int f1, int f2) { //f1 et f2 descripteur
  char buf[t_max];
  int nb_lu = 0;
  int nb_ecrit = 0;
  int nb_total = 0;

  for ( ; ; ) {
    if (nb_lu = read(f1, buf, t_max) <= 0) {
      break;
    }
    if (nb_ecrit = write(f2, buf, nb_lu) == -1) {
      break;
    }
    nb_total = nb_total + nb_ecrit;
  }

  return nb_total;
}

int main(int argc, char const *argv[]) {
  return 0;
}
