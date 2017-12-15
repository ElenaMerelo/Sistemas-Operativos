/*
Enunciado del ejercicio 4: Implementa un programa que lance cinco procesos hijo. Cada uno
de ellos se identificará en la salida estándar, mostrando un mensaje del tipo
Soy el hijo PID.
El proceso padre simplemente tendrá que esperar la finalización de todos sus hijos y
cada vez que detecte la finalización de uno de sus hijos escribirá en la salida
estándar un mensaje del tipo:
Acaba de finalizar mi hijo con <PID>
Sólo me quedan <NUM_HIJOS> hijos vivos
Compilación y enlazado: gcc ejer4.c -o ejer4
Ejecución: ./ejer4
Autora: Elena Merelo Molina
*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
  int num_processes= 5, status;
  pid_t child_process;

  for(int i= 0; i< num_processes; i++){
    if((child_process= fork()) == -1){  //Si no se crea correctamente el hijo imprimimos el correspondiente mensaje de error
      fprintf(stderr, "\n%s\n", "Error en fork" );
      exit(EXIT_FAILURE);
    }
    if(child_process == 0){ //Si se ha creado correctamente
      //Parte ejecutada por el proceso hijo
      fprintf(stdout, "\nSoy el hijo %d con PID %d y PPID %d\n", i, getpid(), getppid());
      exit(0);
    }

    else{
      //Parte ejecutada por el proceso padre
      child_process = wait(&status);
      fprintf(stdout, "\nAcaba de finalizar mi hijo con PID %d\n", child_process);
    }
  }
  /*Como hemos creado 5 hijos, iteramos 5 veces sobre wait, cada vez que se detecta que ha acabado uno,
  informa del PID del hijo que ha acabado e informa de los hijos que quedan vivos. No hace
  falta comprobar dentro del for if(child_process > 0) al haber puesto exit(0) al crear los procesos hijos.*/
/*  for(int i= 4; i>= 0; i--){
    child_process= wait(&status);
    printf("\nHa finalizado mi hijo con PID %d", child_process);
    printf("\nMe quedan %d hijos vivos",i);
  }*/
}
