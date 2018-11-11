/*Enunciado: Este ejercicio se basa en la idea de utilizar varios procesos para
realizar partes de una computación en paralelo. Para ello, deberá construir un
programa que siga el esquema de computación maestro-esclavo, en el cual existen
varios procesos trabajadores (esclavos) idénticos y un único proceso que reparte
trabajo y reúne resultados (maestro). Cada esclavo es capaz de realizar una
computación que le asigne el maestro y enviar a este último los resultados para
que sean mostrados en pantalla por el maestro.
El ejercicio concreto a programar consistirá en el cálculo de los números primos
que hay en un intervalo. Será necesario construir dos programas, maestro y
esclavo. Ten en cuenta la siguiente especificación:
1. El intervalo de números naturales donde calcular los número primos se pasará
como argumento al programa maestro. El maestro creará dos procesos esclavos y
dividirá el intervalo en dos subintervalos de igual tamaño pasando cada
subintervalo como argumento a cada programa esclavo. Por ejemplo, si al maestro
le proporcionamos el intervalo entre 1000 y 2000, entonces un esclavo debe
calcular y devolver los números primos comprendidos en el subintervalo entre
1000 y 1500, y el otro esclavo entre 1501 y 2000. El maestro creará dos cauces
sin nombre y se encargará de su redirección para comunicarse con los procesos
esclavos. El maestro irá recibiendo y mostrando en pantalla (también uno a uno)
los números primos calculados por los esclavos en orden
creciente.
2. El programa esclavo tiene como argumentos el extremo inferior y superior del
intervalo sobre el que buscará números primos. Para identificar un número primo
utiliza el siguiente método concreto: un número n es primo si no es divisible
por ningún k tal que 2 < k <= sqrt(n), donde sqrt corresponde a la función de
cálculo de la raíz cuadrada. El esclavo envía al maestro cada primo
encontrado como un dato entero (4 bytes) que escribe en la salida estándar,
la cuál se tiene que encontrar redireccionada a un cauce sin nombre. Los dos
cauces sin nombre necesarios, cada uno para comunicar cada esclavo con el
maestro, los creará el maestro inicialmente. Una vez que un esclavo haya
calculado y enviado (uno a uno) al maestro todos los primos en su
correspondiente intervalo terminará.
/*Compilación y enlazado: gcc maestro.c -o maestro
Ejecución: ./maestro natural_1 natural_2
Autora: Elena Merelo Molina
*/

#include <stdio.h>  //fprintf() y printf()
#include <unistd.h> //pipe(), exec()
#include <stdlib.h> //atoi(), exit()
#include <errno.h>  //perror()

int main(int argc, char *argv[]){
  int start= atoi(argv[1]), end= atoi(argv[2]);
  pid_t child_process_1, child_process_2;
  int fd_1[2], fd_2[2];
  char middle_char_1[10], middle_char_2[10];

  if(argc != 3 || start > end || start < 0 || end < 0){
    fprintf(stderr, "%s\n","Modo de ejecución: ./ejer5 número_natural_1 número_natural_2, con el primero menor o igual que el segundo" );
    exit(EXIT_FAILURE);
  }

  //Creamos los pipes correspondientes
  if(pipe(fd_1) == -1){
    perror("Error en el primer pipe ");
    exit(EXIT_FAILURE);
  }

  if(pipe(fd_2) == -1){
    perror("Error en el segundo pipe ");
    exit(EXIT_FAILURE);
  }

  //Creamos el primer proceso hijo
  if((child_process_1= fork()) == -1){
    perror("Error en el primer fork");
    exit(EXIT_FAILURE);
  }

  //Dividimos el intervalo pasado como argumento en dos, guardando el valor medio en la siguiente variable:
  int middle_1= (start + end)/2;
  int middle_2= middle_1 +1;
  //------------------------------ ESCLAVO 1 -----------------------------------//
  //Redireccionamos la salida estándar a un cauce sin nombre para los dos procesos hijo
  if (child_process_1 == 0) {
    //Cerramos el descriptor de lectura de cauce en el proceso hijo 1
    close(fd_1[0]);

    /*Duplicamos el descriptor de escritura en cauce en el descriptor correspondiente
    a la salida estándar (stdout), cerrado previamente.*/
    dup2(fd_1[1],STDOUT_FILENO);

    //Convertimos int middle a char middle_[], tipo que acepta execl como argumento
    sprintf(middle_char_1,"%d", middle_1);

    if(execl("./esclavo", "esclavo", argv[1], middle_char_1, NULL) == -1){
      perror("\nError en el primer execl");
      exit(EXIT_FAILURE);
    }
  }
  else { //Parte ejecutada por el padre al ser child_process_1 != 0
    //Cerramos el descriptor de escritura en cauce situado en el proceso padre
    close(fd_1[1]);

    int bytes_1, primes_1;

    printf("\nPrimos del primer proceso hijo: \n");
    while(bytes_1 = read(fd_1[0], &primes_1, sizeof(int)) > 0)
      printf("%d\n", primes_1);

  }


//------------------------------ ESCLAVO 2 -----------------------------------//
  //Creamos el segundo proceso hijo y procedemos de manera análoga al anterior
  if((child_process_2= fork()) == -1){
    perror("Error en el segundo fork");
    exit(EXIT_FAILURE);
  }

  //Realizamos lo mismo para el proceso hijo 2
  if (child_process_2 == 0) { //Parte ejecutada por el hijo
    //Cerramos el descriptor de lectura de cauce
    close(fd_2[0]);

    /*Duplicamos el descriptor de escritura en cauce en el descriptor correspondiente
    a la salida estándar (stdout).*/
    dup2(fd_2[1], STDOUT_FILENO);

    //Convertimos int middle a char middle_[], tipo que acepta execl como argumento
    sprintf(middle_char_2,"%d", middle_2);

    if(execl("./esclavo","esclavo", middle_char_2, argv[2], NULL) == -1){
      perror("\nError en el segundo execl");
      exit(EXIT_FAILURE);
    }
  }

  else { //Parte ejecutada por el padre al ser child_process_1 != 0
    //Cerramos el descriptor de escritura en cauce situado en el proceso padre
    close(fd_2[1]);

    int bytes_2, primes_2;

    printf("\nPrimos del segundo proceso hijo: \n");
    while(bytes_2 = read(fd_2[0], &primes_2, sizeof(int)) > 0)
      printf("%d\n", primes_2);

  }
  exit(1);
}
