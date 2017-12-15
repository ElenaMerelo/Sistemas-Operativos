/*Indica qué tipo de jerarquías de procesos se generan mediante la ejecución de cada
uno de los siguientes fragmentos de código. Comprueba tu solución implementando un
código para generar 20 procesos en cada caso, en donde cada proceso imprima
su PID y el del padre.*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(){
  pid_t childpid;
  int nprocs= 20;
  /*
  Jerarquía de procesos tipo 1. Crea 20 procesos hijos de forma recursiva y cuando terminan
  se sale del bucle, cuando le toca ejecutar al padre se finaliza. Cada hijo es hijo del proceso
  creado anteriormente, se van descendiendo niveles en el árbol con raíz el proceso padre(padre->hijo->hijo...)
  */
  fprintf(stdout, "\n%s\n", "Jerarquía de procesos 1" );
  for (int i= 1; i < nprocs; i++) {
    if ((childpid= fork()) == -1) {
      fprintf(stderr, "\nCould not create child %d: %s", i, strerror(errno));
      exit(EXIT_FAILURE);
    }

    if(childpid== 0) //Si se crea el proceso hijo, ejecutará:
      printf("\nChild process %d created. PID: %d, PPID: %d\n", i+1, getpid(), getppid());

    if (childpid){  //Parte ejecutada por el proceso padre
      wait(NULL); //Espera a que los procesos hijos cambien su estado, ya sea porque han finalizado o obligados a finalizar
      break;//exit(EXIT_FAILURE);
    }
  }
  /*
  Jerarquía de procesos tipo 2. Crea en el mismo nivel hijos de un proceso, cuando
  está ejecutando el proceso hijo no hace nada, pero cuando llega al padre itera
  para crear más hijos.
  */
  pid_t childpid_2;

  for (int i= 1; i < nprocs; i++) {
    if ((childpid_2= fork()) == -1) {
      fprintf(stderr, "Could not create child %d: %s\n",i,strerror(errno));
      exit(-1);
    }
    /*En el guión no incluía esta llamada, pero la ponemos para que cada proceso
    tenga asignado un PID y PPID correcto, les de tiempo a terminar de crearse.*/
    wait(NULL);
    if (!childpid_2)
      //Esta parte se ejecuta por el proceso hijo
      printf("\nChild process %d created. PID: %d, PPID: %d\n", i+1, getpid(), getppid());
      break;
  }
}









//
