/*Implementa un programa en C que tenga como argumento un número entero. Este
programa debe crear un proceso hijo que se encargará de comprobar si dicho número es un
número par o impar e informará al usuario con un mensaje que se enviará por la salida estándar.
A su vez, el proceso padre comprobará si dicho número es divisible por 4, e informará si lo es o
no usando igualmente la salida estándar.*/
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  if(argc != 2){
    printf("\nNúmero de argumentos incorrecto. Ha de introducir un número entero");
    exit(EXIT_FAILURE);
  }

  int n= atoi(argv[1]);
  pid_t pid;

  printf("\nMensaje previo a la ejecución de fork");
  if(pid= fork() < 0){
    perror("\nError en el fork");
    exit(EXIT_FAILURE);
  }
  //Si ejecuta el proceso hijo el programa
  else if(pid == 0)
    printf("\nProceso hijo: %d es un número %s", n, (n%2 == 0)? "par": "impar");

  printf("\nProceso padre: %d %s es divisible por cuatro", n, (n%4 == 0) ? "sí": "no");

}
