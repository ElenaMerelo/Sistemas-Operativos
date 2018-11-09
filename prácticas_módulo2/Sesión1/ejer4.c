/*Ejercicio 4. Define una macro en lenguaje C que tenga la misma funcionalidad que la macro
S_ISREG(mode) usando para ello los flags definidos en <sys/stat.h> para el campo st_mode de
la struct stat, y comprueba que funciona en un programa simple. Consulta en un libro de C o
en internet cómo se especifica una macro con argumento en C.
#define S_ISREG2(mode) ...
Nota: Puede ser interesante para depurar la ejecución de un programa en C que utilice llamadas
al sistema usar la orden strace. Esta orden, en el caso más simple, ejecuta un programa hasta
que finalice e intercepta y muestra las llamadas al sistema que realiza el proceso junto con sus
argumentos y devuelve los valores devueltos en la salida de error estándar o en un archivo si se
especifica la opción -o. Obtén más información con man.*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define S_ISREG2(mode) ( (mode & 0170000) == 0100000)
// #define S_ISREG2(atribute) S_ISREG(atribute.st_mode) --> si ponemos este habrá que poner en la línea 39 atribute.st_mode
#define S_ISREG2(atribute) ((atribute.st_mode & S_IFMT) == S_IFREG)

int main(int argc, char* argv[]){
  int i;
  struct stat atribute;
  char tipoArchivo[30];

  if(argc<2) {
    printf("\nSintaxis de ejecucion: tarea2 [<nombre_archivo>]+\n\n");
    exit(EXIT_FAILURE);
  }

  for(i=1;i<argc;i++) {
    printf("%s: ", argv[i]);
    if(lstat(argv[i],&atribute) < 0) {
      printf("\nError al intentar acceder a los atributos de %s",argv[i]);
      perror("\nError en lstat");
    }
    if(S_ISREG2(atribute))
      strcpy(tipoArchivo,"Regular");
    else
      strcpy(tipoArchivo,"No es un archivo regular");
    printf("%s\n",tipoArchivo);
  }
  return 0;
}
