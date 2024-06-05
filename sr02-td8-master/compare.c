#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

/* Pour compiler :
gcc -o compare compare.c -lm -lpthread -DLinux -DREENTRANT
*/

float sequentiel(int n){

  double start_t;
  double end_t;
  double tot_t;
  int i;
  int j;
  int *tab;


  start_t = clock();

  tab = malloc(sizeof(int)*n);

  // Init tab
  for(i = 0; i < n; i++){
    tab[i] = 1;
  }

  // Traitement
  for(i = 2; i <= floor(sqrt(n)); i++){
    if (tab[i] == 1) {
      for(j = pow(i,2); j < n; j += i){
        tab[j] = 0;
      }
    }
  }

  end_t = clock();
  tot_t = ((end_t - start_t)/(double)CLOCKS_PER_SEC);

  return tot_t;
}


struct calcul_data{
  int deb;
  int fin;
  int num_t;
  int pas;
};

struct init_data{
  int k;
  pthread_t * p_thread;
  int n;
  struct calcul_data *d;
};

int * tab;


// 1 thread
void* calcul_1_thread( void* p_calcul_data){
  int statut;
  int d;
  struct calcul_data *p = p_calcul_data;
  int i;
  int j;
  if(p->num_t == 0){
    d = pow(p->deb, 2);
  }
  else {
    if(p->deb < pow(p->pas, 2))
      d = pow(p->pas,2);
    else
      d = p->deb;
  }
  for(j = d; j <= p->fin; j += p->pas){
    tab[j] = 0;
  }

  pthread_exit(NULL);
}

// creation threads
void* init(void * p_init_data){
  struct init_data *p = p_init_data;
  pthread_attr_t attr;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_attr_init(&attr);

  p->d = malloc(p->k*sizeof(struct calcul_data));
  int i;
  int j;
  int l;
  int fin;

  for(l = 2; l < ceil(sqrt(p->n)); l++){
    if (tab[l] == 1) {
      fin = 0;

      for(i = 0; i < p->k; i++){
        int deb = (fin/l)*l+l;
        fin = (i+1)*ceil((p->n)/(p->k));
        p->d[i].deb = deb;
        p->d[i].fin = fin;
        p->d[i].pas = l;
        p->d[i].num_t = i;
        pthread_create(&(p->p_thread[i]), &attr, calcul_1_thread, &p->d[i]);
      }

      int res;

      for(i = 0; i < p->k; i++){
        pthread_join(p->p_thread[i], NULL);
      }
    }
  }

   pthread_exit(NULL);
}

float parallele(int n, int k){

  int i,res;
  void* statut;
  pthread_t *p_thread;
  pthread_t p_thread_pple;
  double start_t;
  double end_t;
  double tot_t;

  pthread_attr_t attr;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_attr_init(&attr);

  start_t = clock();

  tab = malloc(sizeof(int)*(n+1));
  p_thread = malloc(sizeof(pthread_t)*k);



  // Init tab
  for(i = 0; i < n; i++){
    tab[i] = 1;
  }
  struct init_data data_pple = {k, p_thread, n+1, NULL};
  res = pthread_create(&p_thread_pple, &attr, init, &data_pple);
  if(res != 0){
    perror("Erreur de création ...");
    exit(1);
  }
  pthread_join(p_thread_pple, NULL);


  end_t = clock();
  tot_t = (double)(end_t-start_t)/CLOCKS_PER_SEC;

  return tot_t;
}




int main() {

  int n;
  n = 500000;
  printf("n = %d : \n", n);

  double moy_seq = 0;
  double simple_Seq = sequentiel(n);
  for (int i = 0; i < 10; i++) {
    moy_seq += sequentiel(n);
  }
  moy_seq = moy_seq/10;

  double moy_par[7];
  double simple_par[7];
  for (int k = 1; k < 8; k++){
    moy_par[k-1] = 0;
    simple_par[k-1]=0;
  }
  for (int k = 1; k < 8; k++){
    for (int i = 0; i < 10; i++){
      moy_par[k-1] += parallele(n, k);
    }
    simple_par[k-1] = parallele(n,k);
  }
  for (int k = 1; k < 8; k++){
    moy_par[k-1] = moy_par[k-1]/10;
  }
    printf("Résultats simples : \nseq=%f, \npar1=%f, \npar2=%f, \npar3=%f, \npar4=%f, \npar5=%f, \npar6=%f, \npar7=%f \n", simple_Seq,simple_par[0], simple_par[1], simple_par[2], simple_par[3], simple_par[4], simple_par[5], simple_par[6]);
    printf("Résultats (moyenne de 10 lancements) : \nseq=%f, \npar1=%f, \npar2=%f, \npar3=%f, \npar4=%f, \npar5=%f, \npar6=%f, \npar7=%f \n\n\n", moy_seq,moy_par[0], moy_par[1], moy_par[2], moy_par[3], moy_par[4], moy_par[5], moy_par[6]);
    printf("------------------------------------------------------------------------\n");


  n = 1000000;
  printf("n = %d : \n", n);

  moy_seq = 0;
  simple_Seq = sequentiel(n);
  for (int i = 0; i < 10; i++) {
    moy_seq += sequentiel(n);
  }
  moy_seq = moy_seq/10;

  for (int k = 1; k < 8; k++){
    moy_par[k-1] = 0;
    simple_par[k-1]=0;
  }
  for (int k = 1; k < 8; k++){
    for (int i = 0; i < 10; i++){
      moy_par[k-1] += parallele(n, k);
    }
    simple_par[k-1] = parallele(n,k);
  }
  for (int k = 1; k < 8; k++){
    moy_par[k-1] = moy_par[k-1]/10;
  }

  printf("Résultats simples : \nseq=%f, \npar1=%f, \npar2=%f, \npar3=%f, \npar4=%f, \npar5=%f, \npar6=%f, \npar7=%f \n", simple_Seq,simple_par[0], simple_par[1], simple_par[2], simple_par[3], simple_par[4], simple_par[5], simple_par[6]);
  printf("Résultats (moyenne de 10 lancements) : \nseq=%f, \npar1=%f, \npar2=%f, \npar3=%f, \npar4=%f, \npar5=%f, \npar6=%f, \npar7=%f \n\n\n", moy_seq,moy_par[0], moy_par[1], moy_par[2], moy_par[3], moy_par[4], moy_par[5], moy_par[6]);
  printf("------------------------------------------------------------------------\n");

  n = 2000000;
  printf("n = %d : \n", n);

  moy_seq = 0;
  simple_Seq = sequentiel(n);
  for (int i = 0; i < 10; i++) {
    moy_seq += sequentiel(n);
  }
  moy_seq = moy_seq/10;

  for (int k = 1; k < 8; k++){
    moy_par[k-1] = 0;
    simple_par[k-1]=0;
  }
  for (int k = 1; k < 8; k++){
    for (int i = 0; i < 10 ; i++){
      moy_par[k-1] += parallele(n, k);
    }
    simple_par[k-1] = parallele(n,k);
  }
  for (int k = 1; k < 8; k++){
    moy_par[k-1] = moy_par[k-1]/10;
  }

  printf("Résultats simples : \nseq=%f, \npar1=%f, \npar2=%f, \npar3=%f, \npar4=%f, \npar5=%f, \npar6=%f, \npar7=%f \n", simple_Seq,simple_par[0], simple_par[1], simple_par[2], simple_par[3], simple_par[4], simple_par[5], simple_par[6]);
  printf("Résultats (moyenne de 10 lancements) : \nseq=%f, \npar1=%f, \npar2=%f, \npar3=%f, \npar4=%f, \npar5=%f, \npar6=%f, \npar7=%f \n\n\n", moy_seq,moy_par[0], moy_par[1], moy_par[2], moy_par[3], moy_par[4], moy_par[5], moy_par[6]);
  printf("------------------------------------------------------------------------\n");


  n = 4000000;
  printf("n = %d : \n", n);

  moy_seq = 0;
  simple_Seq = sequentiel(n);
  for (int i = 0; i < 10; i++) {
    moy_seq += sequentiel(n);
  }
  moy_seq = moy_seq/10;

  for (int k = 1; k < 8; k++){
    moy_par[k-1] = 0;
    simple_par[k-1]=0;
  }
  for (int k = 1; k < 8; k++){
    for (int i = 0; i < 10; i++){
      moy_par[k-1] += parallele(n, k);
    }
    simple_par[k-1] = parallele(n,k);
  }
  for (int k = 1; k < 8; k++){
    moy_par[k-1] = moy_par[k-1]/10;
  }

  printf("Résultats simples : \nseq=%f, \npar1=%f, \npar2=%f, \npar3=%f, \npar4=%f, \npar5=%f, \npar6=%f, \npar7=%f \n", simple_Seq,simple_par[0], simple_par[1], simple_par[2], simple_par[3], simple_par[4], simple_par[5], simple_par[6]);
  printf("Résultats (moyenne de 10 lancements) : \nseq=%f, \npar1=%f, \npar2=%f, \npar3=%f, \npar4=%f, \npar5=%f, \npar6=%f, \npar7=%f \n\n\n", moy_seq,moy_par[0], moy_par[1], moy_par[2], moy_par[3], moy_par[4], moy_par[5], moy_par[6]);
  printf("------------------------------------------------------------------------\n");
  return 0;
}
