/*
get_signal.c
Trabajo con llamadas al sistema del Subsistema de Procesos conforme a POSIX 2.10
Utilización de la llamada sigaction para cambiar el comportamiento del proceso
frente a la recepción de una señal.
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

//Determina cuál ha sido la señal recibida
static void sig_USR_hdlr(int sigNum){
  if(sigNum == SIGUSR1)
    printf("\nRecibida la señal SIGUSR1\n\n");
  else if(sigNum == SIGUSR2)
    printf("\nRecibida la señal SIGUSR2\n\n");
}

int main(int argc, char *argv[]){
  struct sigaction sig_USR_nact;

  //Para que escriba en la salida estándar sin buffering
  if(setvbuf(stdout,NULL,_IONBF,0))
    perror("\nError en setvbuf");

  //Inicializamos la estructura sig_USR_na para especificar la nueva
  //acción para la señal.
  sig_USR_nact.sa_handler= sig_USR_hdlr;

  //Inicializa el conjunto de señales dado al conjunto vacío.
  sigemptyset (&sig_USR_nact.sa_mask);
  sig_USR_nact.sa_flags = 0;

  /*Establecemos un manejador de señal para SIGUSR1. Con
  sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)
  cambiamos el comportamiento del proceso al recibir la señal SIGUSR1. La nueva acción
  que realice será sig_USR_nact.*/
  if( sigaction(SIGUSR1,&sig_USR_nact,NULL) <0){
    perror("\nError al intentar establecer el manejador de señal para SIGUSR1");
    exit(-1);
  }

  //Hacemos lo mismo para SIGUSR2
  if( sigaction(SIGUSR2,&sig_USR_nact,NULL) <0){
    perror("\nError al intentar establecer el manejador de señal para SIGUSR2");
    exit(-1);
  }
  for(;;){ }  //equivalente a while(1)
}
