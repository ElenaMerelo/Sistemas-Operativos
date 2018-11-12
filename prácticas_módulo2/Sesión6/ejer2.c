/*
Reescribir el programa que implemente un encauzamiento de dos órdenes pero
utilizando fcntl. Este programa admitirá tres argumentos. El primer argumento y el tercero
serán dos órdenes de Linux. El segundo argumento será el carácter “|”. El programa deberá
ahora hacer la redirección de la salida de la orden indicada por el primer argumento hacia el
cauce, y redireccionar la entrada estándar de la segunda orden desde el cauce. Por ejemplo,
para simular el encauzamiento ls|sort, ejecutaríamos nuestro programa como:
$> ./mi_programa2
ls “|” sort

Autora: Elena Merelo Molina
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
  if(argc != 4){
    printf("\nNúmero de argumentos incorrecto");
    exit(EXIT_FAILURE);
  }

  if(strcmp(argv[2], "|") != 0 ){
    printf("\nHa de introducir una orden linux, un | y otra orden linux.");
    exit(EXIT_FAILURE);
  }

  int fd[2];
  pid_t PID;

  // Creamos un pipe
  pipe(fd);

  // Lanzamos el proceso hijo
  if((PID= fork()) < 0){
    perror("\nError en fork");
    exit(EXIT_FAILURE);
  }

  //Si estamos dentro del proceso hijo ejecutamos la primera orden linux
  if(PID == 0){
    close(fd[0]);

    //Cerramos la salida estándar asignada al proceso llamador, para que se escriba en el archivo y no ahí
    close(STDOUT_FILENO);

    // Duplicamos fd en el descriptor 1, STDOUT_FILENO
    if(fcntl(fd[1], F_DUPFD, STDOUT_FILENO) == -1) {
      perror ("\nFallo en fcntl");
      exit(EXIT_FAILURE);
    }
    execlp(argv[1], argv[1], NULL);
  }
  else{
    close(fd[1]);

    //Cerramos la salida estándar asignada al proceso llamador, para que se escriba en el archivo y no ahí
    close(STDIN_FILENO);

    // Duplicamos fd en el descriptor 1, STDOUT_FILENO
    if(fcntl(fd[0], F_DUPFD, STDIN_FILENO) == -1) {
      perror ("\nFallo en fcntl");
      exit(EXIT_FAILURE);
    }
    execlp(argv[3], argv[3], NULL);
  }
}
