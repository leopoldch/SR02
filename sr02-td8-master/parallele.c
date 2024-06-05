#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

/* Pour compiler :
gcc -o parallele parallele.c -lm -lpthread -DLinux -DREENTRANT
*/

// Struct data
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
      d = pow(p->pas, 2);
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

double main(){

  int n;
  int k;
  int i;
  int res;
  void* statut;
  pthread_t *p_thread;
  pthread_t p_thread_ppale;
  double start_t;
  double end_t;
  double tot_t;

  pthread_attr_t attr;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_attr_init(&attr);

  printf("Jusqu'à combien voulez-vous faire le calcul ? ... \n");
  scanf("%d", &n);

  printf("Sur combien de threads voulez-vous travailler ? ...");
  scanf("%d", &k);

  start_t = clock();

  tab = malloc(sizeof(int)*(n+1));
  p_thread = malloc(sizeof(pthread_t)*k);



  // Init tab
  for(i = 0; i < n; i++){
    tab[i] = 1;
  }
  struct init_data data_pple = {k, p_thread, n+1, NULL};
  res=pthread_create(&p_thread_ppale, &attr, init, &data_pple);
  if(res != 0){
    perror("Erreur de création ...");
    exit(1);
  }
  pthread_join(p_thread_ppale, NULL);


  //Afficher
 for(i = 0; i < n; i++){
    printf("tab[%d] = %d \n", i, tab[i]);
  }

  end_t = clock();
  tot_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

  printf("Début : %f \n", start_t);
  printf("Fin : %f \n", end_t);
  printf("Temps d'execution : %f \n", tot_t);

  return tot_t;
}
