//Escribe un programa que suspenda la ejecución del proceso actual hasta que se reciba la señal SIGUSR1.

#include <signal.h>
#include <stdio.h>

int main(){
  sigset_t new_mask;

  //Inicializamos la nueva máscara de señales a todas las posibles
  sigfillset(&new_mask);

  //Eliminamos del conjunto la señal SIGUSR1
  sigdelset(&new_mask, SIGUSR1);

  /*Espera a la señal SIGUSR1, ya que realmente está esperando a todas las señales
  excepto las del set, que son todas menos SIGUSR1.*/
  sigsuspend(&new_mask);

}
