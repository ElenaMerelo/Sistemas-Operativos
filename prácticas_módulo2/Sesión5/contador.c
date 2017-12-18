/*Enunciado: Escribe un programa en C llamado contador, tal que cada vez que reciba una
señal que se pueda manejar, muestre por pantalla la señal y el número de veces que se ha
recibido ese tipo de señal, y un mensaje inicial indicando las señales que no puede manejar.*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*Vector en el que iremos almacenando las veces que aparece una señal. Le ponemos
ese tamaño ya que hay 27 señales, desde la 1 a la 27.*/
int number_of_times[28]= {0};

static void signal_handler(int signum){
  number_of_times[signum]++;
  printf("\nLa señal %d se ha recibido %d veces\n", signum, number_of_times[signum]);
  signal(signum, SIG_IGN);  //Ignora así la señal que recibe, solo cuenta el número de veces que se ha invocado
}

int main(int argc, char *argv[]){

  //Informamos de las señales que no podemos manejar
  printf("\nNo puedo manejar la señal %d", SIGKILL);
  printf("\nNo puedo manejar la señal %d", SIGSTOP);
  printf("\nEsperando el envío de señales...\n");

  //Para que escriba en la salida estándar sin buffering
  if(setvbuf(stdout,NULL,_IONBF,0))
    perror("\nError en setvbuf");

  for(int i= 0; i< 28; i++){
    if(signal(i, signal_handler) < 0){
      printf("\nNo puedo manejar la señal %d\n", i);
      exit(EXIT_FAILURE);
    }
  }
  while(1); //Para que siga ejecutándose hasta que reciba una señal que termine el proceso
}
