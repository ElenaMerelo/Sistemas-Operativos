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
