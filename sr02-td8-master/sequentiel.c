#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/* Pour compiler :
gcc -o sequentiel sequentiel.c -lm
*/

double main(){

  double start_t, end_t, tot_t;
  int n;
  int i;
  int j;
  int *tab;

  printf("Entrez un nombre ... \n");
  scanf("%d", &n);

  start_t = clock();

  tab = malloc(sizeof(int)*n);

  //Init tab
  for(i = 0; i < n; i++){
    tab[i] = 1;
  }

  // traitement
  for(i = 2; i <= floor(sqrt(n)); i++){
    if (tab[i] == 1) {
      for(j = pow(i,2); j < n; j += i){
        tab[j] = 0;
      }
    }
  }

  // Afficher
  for(i = 0; i < n; i++){
    printf("tab[%d] = %d \n", i, tab[i]);
  }

  end_t = clock();
  tot_t = ((end_t - start_t)/(double)CLOCKS_PER_SEC);

  printf("Début : %f \n", start_t);
  printf("Fin : %f \n", end_t);
  printf("Temps d'execution : %f \n", tot_t);

  return tot_t;
}
