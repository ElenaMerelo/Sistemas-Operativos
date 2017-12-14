/* Enunciado: Implementa un programa en C que tenga como argumento un número entero. Este
programa debe crear un proceso hijo que se encargará de comprobar si dicho número es un
número par o impar e informará al usuario con un mensaje que se enviará por la salida estándar.
A su vez, el proceso padre comprobará si dicho número es divisible por 4, e informará si lo es o
no usando igualmente la salida estándar.
Compilación y enlazado: gcc ejer1.c -o ejer1
Ejecución: ./ejer1 *whatever*
Autora: Elena Merelo Molina
*/
#include <unistd.h> //Para fork()
//#include <errno.h>
#include <stdio.h>  //Para printf()
#include <ctype.h>  //Para isdigit()
#include <string.h> //Para poder usar strlen()
#include <stdlib.h> //Para exit()

//Comprueba si number es par o impar
void child_process(int number){
  printf("\nProceso hijo, pid: %d, ppid: %d", getpid(), getppid());
  printf("\nEl número %d es %s", number, (number%2 == 0)? "par": "impar");
}
//Comprueba si el número es divisible por cuatro
void parent_process(int number){
  printf("\nProceso padre, pid: %d, ppid: %d", getpid(), getppid());
  printf("\nEl número %d %s divisible por cuatro", number, (number%4 == 0)? "sí": "no");
}

int main(int argc, char* argv[]){
  pid_t pid;

  if(argc != 2 || atoi(argv[1]) == 0){
    printf("\nHa de introducir un número entero");
    exit(EXIT_FAILURE);
  }
  if((pid= fork()) == 0)  //Si se crea el proceso hijo
    child_process(atoi(argv[1]));

  parent_process(atoi(argv[1]));
}
