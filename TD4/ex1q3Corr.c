#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#define BLKSIZE 1024
#define PERM (S_IRUSR | S_IWUSR)


int copiefichier(int f1, int f2) {
char buf[BLKSIZE];
int octetslus, octetsecri; int totaloct = 0;
for ( ; ; ) {
if ((octetslus = read(f1, buf, BLKSIZE)) <= 0)
break;
if ((octetsecri = write(f2, buf, octetslus)) == -1)
break;
totaloct += octetsecri;
}
return totaloct;
}


int detachandremove(int shmid, void *shmaddr) {
int error = 0;
if (shmdt(shmaddr) == -1)
error = errno;
if ((shmctl(shmid, IPC_RMID, NULL) == -1) && !error)
error = errno;
if (!error)
return 0;
errno = error;
return -1;
}


int main(int argc, char *argv[]) {
int octetslus;
int childpid;
int fd, fd1, fd2;
int id;
int *sharedtotal;
int totalbytes = 0;
if (argc != 3) {
fprintf(stderr, "Usage: %s fichier 1 fichier 2\n", argv[0]);
return 1;
}
if (((fd1 = open(argv[1], O_RDONLY)) == -1) ||
((fd2 = open(argv[2], O_RDONLY)) == -1)) {
perror("Echec de l'ouverture du fichier");
return 1;
}
if ((id = shmget(IPC_PRIVATE, sizeof(int), PERM)) == -1) {
perror("Echec de de creation d'un segment");
return 1;
}
if ((sharedtotal = (int *)shmat(id, NULL, 0)) == (void *)-1) {
perror("Echec de l'attachement au segment");

}
if ((childpid = fork()) == -1) {
perror("Echec de fork");
if (detachandremove(id, sharedtotal) == -1)
perror("Echec de detruire le segment");
return 1;
}
if (childpid > 0) /* code du parent */
fd = fd1;
else
fd = fd2;
while ((octetslus = copiefichier(fd, STDOUT_FILENO)) > 0)
totalbytes += octetslus;
if (childpid == 0) /* child code */
{ 
*sharedtotal = totalbytes;
return 0;
}
if (wait(NULL) == -1)
perror("Echec de wait");
else {
fprintf(stderr, "Octets copiés: %8d par le parent\n", totalbytes);
fprintf(stderr, " %8d par le fils\n", *sharedtotal);
fprintf(stderr, " %8d total\n", totalbytes + *sharedtotal);
}
if (detachandremove(id, sharedtotal) == -1) {
("Echec de detruire le segment");

}
return 0;
}