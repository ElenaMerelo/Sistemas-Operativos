/*Ejercicio 5. Implementa una modificación sobre el anterior programa en la que el proceso
padre espera primero a los hijos creados en orden impar (1º,3º,5º) y después a los hijos pares (2º
y 4º).
Compilación y enlazado: gcc ejer5.c -o ejer5
Ejecución: ./ejer5
Autora: Elena Merelo Molina
*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
  int num_processes= 5, status, children[num_processes];  //En éste último vamos almacenando los PIDs de los procesos hijos
  //pid_t child_process;

  for(int i= 0; i< num_processes; i++){
    if((children[i]= fork()) == -1){  //Si no se crea correctamente el hijo imprimimos el correspondiente mensaje de error
      fprintf(stderr, "\n%s\n", "Error en fork" );
      exit(EXIT_FAILURE);
    }
    if(children[i] == 0){ //Si se ha creado correctamente
      //Parte ejecutada por el proceso hijo
      fprintf(stdout, "\nSoy el hijo %d con PID %d y PPID %d\n", i, getpid(), getppid());
      exit(0);
    }
  }
  /*Como hemos creado 5 hijos, iteramos 5 veces sobre wait, cada vez que se detecta que ha acabado uno,
  informa del PID del hijo que ha acabado e informa de los hijos que quedan vivos. No hace
  falta comprobar dentro del for if(child_process > 0) al haber puesto exit(0) al crear los procesos hijos.*/
  //Esperamos a los hijos impares, que se corresponden con las posiciones pares del vector
  for(int i= 0; i< 5; i++){
    if(i%2 == 0){
      waitpid(children[i], &status, 0);
      printf("\nHa finalizado mi hijo con PID %d y estado %d\n", children[i], status);
      printf("\nMe quedan %d hijos vivos, éste es el hijo %d\n", --num_processes, i+1);
    }
  }
  for(int i= 0; i< 5; i++){
    //Esperamos a los hijos pares, que se corresponden con las posiciones impares del vector
    if(i%2 != 0){
      waitpid(children[i], &status, 0);
      printf("\nHa finalizado mi hijo con PID %d y estado %d\n", children[i], status);
      printf("\nMe quedan %d hijos vivos, éste es el hijo %d\n", --num_processes, i+1);
    }
  }
}
