#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>

char buffer1[80];

int main(int argc, char *argv[]){
  int fd, fd_o, i=0;
  if(argc != 2)
    fd=0;
  else{
    if( (fd=open(argv[1],O_CREAT|O_RDONLY,S_IRUSR|S_IWUSR))<0) {
      printf("\nError %d en open",errno);
      perror("\nError en open");
      exit(-1);
    }
  }

  if( (fd_o=open("salida.txt",O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR))<0) {
    printf("\nError %d en open",errno);
    perror("\nError en open");
    exit(-1);
  }

  struct stat atributos;
  if(lstat(argv[1], &atributos) < 0)
    printf("%s\n", "Error en lstat" );

  long long int n= atributos.st_size/80;
  char buffer2[]="El número de bloques es: ";
  dprintf(fd_o, "%s : %lld", buffer2, n);

  char buffer3[]="\nBloque";

  while(read(fd, buffer1, 80) != 0){
    dprintf(fd_o, "%s %d : %s", buffer3, i++, "\n");
    if(write(fd_o, buffer1, 80) != 80) {
      perror("\nError write");
      exit(-1);
    }

  }
  close(fd);
  close(fd_o);
  return 0;
}
