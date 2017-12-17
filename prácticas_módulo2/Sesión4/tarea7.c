/*
tarea7.c
Programa ilustrativo del uso de pipes y la redirección de entrada y
salida estándar: "ls | sort"
*/
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
int main(int argc, char *argv[]){
  int fd[2];
  pid_t PID;
  pipe(fd); // Llamada al sistema para crear un pipe
  if ( (PID= fork())<0) {
    perror("fork");
    exit(-1);
  }
  if(PID == 0) { // ls
    //Establecemos la dirección del flujo de datos en el cauce cerrando
    // el descriptor de lectura de cauce en el proceso hijo
    close(fd[0]); //Así, redirigimos la salida estándar para enviar datos al cauce

    //Cerramos la salida estándar del proceso hijo
    close(STDOUT_FILENO);
    //Duplicamos el descriptor de escritura en cauce en el descriptor
    //correspondiente a la salida estándar (stdout)
    dup(fd[1]);
    execlp("ls","ls",NULL);
  }
  else { // sort. Estamos en el proceso padre porque PID != 0
    //Se establece la dirección del flujo de datos en el cauce cerrando
    // el descriptor de escritura en el cauce del proceso padre.
    close(fd[1]);
    //Redirigimos la entrada estándar para tomar los datos del cauce.
    //Cerramos la entrada estándar del proceso padre
    close(STDIN_FILENO);
    //Duplicamos el descriptor de lectura de cauce en el descriptor
    //correspondiente a la entrada estándar (stdin)
    dup(fd[0]);
    execlp("sort","sort",NULL);
  }
  return(0);
}
