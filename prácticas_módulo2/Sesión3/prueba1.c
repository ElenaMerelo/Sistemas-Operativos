#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
void main(void){
  printf("Identificador de proceso: %d\n", getpid());
  printf("Identificador del proceso padre: %d\n", getppid());
  printf("Identificador de usuario real: %d\n", getuid());
  printf("Identificador de usuario efectivo: %d\n", geteuid());
  printf("Identificador de grupo real: %d\n", getgid());
  printf("Identificador de grupo efectivo: %d\n", getegid());
}
