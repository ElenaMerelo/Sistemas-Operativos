/*Compilación y enlazado: gcc esclavo.c -o esclavo -lm
Se ejecuta el programa principal, maestro.c
Autora: Elena Merelo Molina
*/
#include <stdio.h>  //familia de funciones printf()
#include <stdlib.h> //exit(), atoi
#include <math.h>   //necesita -lm al enlazar
#include <unistd.h> //write()

//Devuelve 0 si el número no es primo, 1 si sí lo es
int is_prime(int number){
  //Si el número tiene algún divisor en el intervalo [2,sqrt(number)] no es primo
  for(int i= 2; i<= sqrt(number); i++){
    if(number % i == 0)
      return 0;
  }
  return 1;
}
int main(int argc, char *argv[]){
  if(argc != 3 || argv[1] > argv[2]){
    fprintf(stderr, "%s\n","Modo de ejecución: ./ejer5 número_natural_1 número_natural_2, con el primero menor o igual que el segundo" );
    exit(EXIT_FAILURE);
  }

  int start= atoi(argv[1]);
  int end= atoi(argv[2]);
  //Recorremos el intervalo pasado como argumento buscando los primos que contiene
  for(int i= start; i< end; i++){
    /*Enviamos cada primo encontrado al maestro como un entero que escribe en
    la salida estándar, la cual se encuentra ya redireccionada a un cauce sin nombre,
    creado por el maestro.*/
    if(is_prime(i)){
      printf("\nEl número %d es primo\n", i);
      write(STDOUT_FILENO, &i, sizeof(int));
    }
  }
  exit(0);
}
