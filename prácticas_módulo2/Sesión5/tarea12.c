// tarea12.c

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static int signal_recibida = 0;

static void manejador (int sig){
  signal_recibida = 1;
}

int main (int argc, char *argv[]){
  //Creamos dos conjuntos de máscaras y una estructura sigaction
  sigset_t conjunto_mascaras;
  sigset_t conj_mascaras_original;
  struct sigaction act;

  //Iniciamos a 0 todos los elementos de la estructura act. Inicia en 0 los n primeros bits de la estructura atc, donde n = sizeof(act). Es decir, la pone entera a 0. Para más información, consultar el man.
  memset (&act, 0, sizeof(act));

  //Asociamos la función manejador creada al sa_handler de act.
  act.sa_handler = manejador;

  //Se asocia la función manejador a la llamada SIGTERM, tal y como se explicó previamente.
  if (sigaction(SIGTERM, &act, 0)) {
      perror ("sigaction");
      exit(EXIT_FAILURE);
  }

  //Iniciamos un nuevo conjunto de mascaras
  sigemptyset (&conjunto_mascaras);
  //Añadimos SIGTERM al conjunto de mascaras
  sigaddset (&conjunto_mascaras, SIGTERM);

  //Bloqueamos SIGTERM usando la función sigprocmask. Al finalizar el código se explica más detenidamente.
  if (sigprocmask(SIG_BLOCK, &conjunto_mascaras, &conj_mascaras_original) < 0) {
     perror ("primer sigprocmask");
     exit(EXIT_FAILURE);
  }

  //10 segundos en los que podemos mandarle la señal SIGTERM que la tendrá en cuenta cuando la vuelva a activar
  sleep (10);

  //El proceso toma como suya la máscara conj_mascaras_original, así restauramos la señal SIGTERM
  if (sigprocmask(SIG_SETMASK, &conj_mascaras_original, NULL) < 0) {
     perror ("segundo sigprocmask");
     exit(EXIT_FAILURE);
     }

  sleep (1);

 if (signal_recibida)
     printf ("\nSenal recibida\n");
 exit(EXIT_SUCCESS);
}
