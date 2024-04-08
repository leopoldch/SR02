#include<stdio.h>
#include<unistd.h>
#include <fcntl.h>
#include<string.h>
#include<errno.h>

#define BLKSIZE 1024

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



int main(int argc, char *argv[]) {
int octetslus; int childpid;
int fd, fd1, fd2;
if (argc != 3) {
fprintf(stderr, "Usage: %s fichier 1 fichier 2\n", argv[0]);
return 1;
}
if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
fprintf(stderr, "Echec de l'ouverture du fichier %s:%s\n", argv[1], strerror(errno));
return 1;
}
if ((fd2 = open(argv[2], O_RDONLY)) == -1) {
fprintf(stderr, "Echec de l'ouverture du fichier %s:%s\n", argv[2], strerror(errno));
return 1;
}
if ((childpid = fork()) == -1) {
perror(" ((parent) Echec de fork()");
return 1;
}uh
if (childpid > 0)
/* code du parent */
fd = fd1;
else
fd = fd2;
octetslus = copiefichier(fd, fileno(stdout));
fprintf(stderr, " Octes lus: %d\n", octetslus);
return 0;
}