# TD6 SR02 



## Exercice 1
```
def philosophe(i):
    si fourchette(i) && fouchette(i+1)
        prendre_fourchette(i)
        prendre_fourchette(i+1)
        manger()
        reposer_fourchette(i)
        reposer_fourchette(i+1)

    sinon tant que non fourchette(i) || non fouchette(i+1)
        penser()

    prendre_fourchette(i)
    prendre_fourchette(i+1)
    manger()
    reposer_fourchette(i)
    reposer_fourchette(i+1)
```

> on arrive à un bloquage si la personne à la droite de i mange puis la personne à la gauche de i mange etc 
> le philosphe i ne pourrait pas manger

#### solution grâce aux sémaphores 

Nombre de place disponible à table 

````

#include semaphore 

sem_t semaphore;

sem_init(&semaphore,0,1);


sem_destroy(&semaphore)

````



