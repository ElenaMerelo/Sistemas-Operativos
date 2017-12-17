## Sesión 5. Llamadas al sistema para gestión y control de señales.
### Actividad 5.1. Trabajo con las llamadas al sistema sigaction y kill.
A continuación se muestra el código fuente de dos programas. El programa send_signal permite el envío de una señal a un proceso identificado por medio de su PID. El programa get_signal se ejecuta en background y permite la recepción de señales.
**Ejercicio 1.** Compila y ejecuta los siguientes programas y trata de entender su funcionamiento.
~~~c
/*
send_signal.c
Trabajo con llamadas al sistema del Subsistema de Procesos conforme a POSIX 2.10
Utilización de la llamada kill para enviar una señal:
0: SIGTERM
1: SIGUSR1
2: SIGUSR2
a un proceso cuyo identificador de proceso es PID.
SINTAXIS: send_signal [012] <PID>*/

#include <sys/types.h> //POSIX Standard: 2.6 Primitive System Data Types
#include<limits.h> //Incluye <bits/posix1_lim.h> POSIX Standard: 2.9.2 //Minimum Values Added to <limits.h> y <bits/posix2_lim.h>
#include <unistd.h> //POSIX Standard: 2.10 Symbolic Constants
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

int main(int argc, char* argv[]){
  long int pid;
  int signal;

  if(argc != 3) {
    printf("\nSintaxis de ejecución: ./send_signal [012] <PID>\n\n");
    exit(-1);
  }
  pid= strtol(argv[2],NULL,10); //Pasamos lo introducido como argumento a un decimal

  /*LONG_MIN y LONG_MAX son, respectivamente, los menores y mayores valores que puede tomar un long int. Por ello hacemos esta comprobación, porque son números excesivos para el programa.*/
  if(pid == LONG_MIN || pid == LONG_MAX){
    if(pid == LONG_MIN)
      printf("\nError por desbordamiento inferior LONG_MIN %d",pid);
    else
      printf("\nError por desbordamiento superior LONG_MAX %d",pid);
      perror("\nError en strtol");
      exit(-1);
  }
  signal=atoi(argv[1]); //Convertimos a entero el primer argumento del programa

  /*Con kill, si se introduce como argv[1] un 0, manda una señal de terminación al proceso con el ID especificado por pid(si pid es positivo),a todos los procesos en el grupo de procesos del proceso invocante(si pid es 0), a todos los procesos para los que el proceso invocante tiene permiso de enviar señales(si pid es -1) o a todos los procesos del grupo de procesos cuyo ID sea -pid(si pid es menor que -1).*/
  switch(signal) {
  case 0: //SIGTERM
      kill(pid,SIGTERM);  //Manda la señal de terminación
      break;
    case 1: //SIGUSR1
      kill(pid,SIGUSR1);  //Manda una señal definida por el usuario
      break;
    case 2: //SIGUSR2
      kill(pid,SIGUSR2);  //Segunda señal definida por el usuario
      break;
    default : // not in [012]
      printf("\n No puedo enviar ese tipo de señal");
  }
}
~~~
**Con este programa, dependiendo del número introducido por el usuario se mandan unas señales u otras a proceso, grupo de procesos,..., según sea pid.**

~~~c
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
  if(setvbuf(stdout,NULL,_ IONBF,0))
    perror("\nError en setvbuf");

  //Inicializamos la estructura sig_USR_na para especificar la nueva acción para la señal.
  sig_USR_nact.sa_handler= sig_USR_hdlr;

  //'sigemptyset' inicia el conjunto de señales dado al conjunto vacío.
  sigemptyset (&sig_USR_nact.sa_mask);
  sig_USR_nact.sa_flags = 0;
  //Establecer mi manejador particular de señal para SIGUSR1
  if( sigaction(SIGUSR1,&sig_USR_nact,NULL) <0){
    perror("\nError al intentar establecer el manejador de señal para SIGUSR1");
    exit(-1);
  }

  //Establecer mi manejador particular de señal para SIGUSR2
  if( sigaction(SIGUSR2,&sig_USR_nact,NULL) <0){
    perror("\nError al intentar establecer el manejador de señal para SIGUSR2");
    exit(-1);
  }
  for(;;){ }  //equivalente a while(1)
}








### Extra:
Los siguientes ejemplos ilustran el uso de la llamada al sistema sigaction para establecer un manejador para la señal SIGINT que se genera cuando se pulsa <CTRL+C>.
~~~c
// tarea9.c
#include <stdio.h>
#include <signal.h>
int main(){
  struct sigaction sa;
  sa.sa_handler = SIG_IGN; // ignora la señal
  sigemptyset(&sa.sa_mask);
  //Reiniciar las funciones que hayan sido interrumpidas por un manejador
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    printf("Error en el manejador");

  while(1);
}

// tarea10.c
#include <stdio.h>
#include <signal.h>
static int s_recibida=0;
static void handler (int signum){
  printf("\n Nueva acción del manejador \n");
  s_recibida++;
}

int main(){
  struct sigaction sa;
  sa.sa_handler = handler; // establece el manejador a handler
  sigemptyset(&sa.sa_mask);
  //Reiniciar las funciones que hayan sido interrumpidas por un manejador
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    printf("error en el manejador");
  while(s_recibida<3);
}
