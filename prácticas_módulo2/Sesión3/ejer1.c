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
#include <stdio.h>  //Para printf()
#include <ctype.h>  //Para isdigit()
#include <string.h> //Para poder usar strlen()
#include <stdlib.h> //Para exit()

//Comprueba si number es par o impar
void child_process(int number){
  printf("\nProceso hijo~ pid: %d, ppid: %d", getpid(), getppid());
  printf("\nEl número %d es %s", number, (number%2 == 0)? "par": "impar");
}
//Comprueba si el número es divisible por cuatro
void parent_process(int number){
  /*Espera a que el estado del proceso hijo cambie para continuar(ya sea porque ha
  finalizado o haya sido bloqueado o obligado a parar por una señal). En el caso de un proceso hijo
  terminado, hacer wait permite al sistema liberar los recursos asociados a él. Si no se hace
  wait el proceso hijo finalizado se mantiene en estado zombie.(Cabecera de la función: pid_t wait(int *status);)
  */
  wait(NULL);
  printf("\nProceso padre~ pid: %d, ppid: %d", getpid(), getppid());
  printf("\nEl número %d %s divisible por cuatro", number, (number%4 == 0)? "sí": "no");
}

int main(int argc, char* argv[]){
  pid_t pid;

  if(argc != 2 || atoi(argv[1]) == 0){
    printf("\nHa de introducir un número entero");
    exit(EXIT_FAILURE);
  }

  pid= fork();
  if(pid< 0){
    perror("\nError en el fork()");
    exit(EXIT_FAILURE);
  }
  else if(pid == 0)  //Si se crea el proceso hijo
    child_process(atoi(argv[1]));
  else
    parent_process(strtoul(argv[1], 0, 10)); //Equivalente a atoi(argv[1])

}
