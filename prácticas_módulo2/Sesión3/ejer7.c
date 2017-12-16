/*Enunciado: Escribe un programa que acepte como argumentos el nombre de un programa,
sus argumentos si los tiene, y opcionalmente la cadena “bg”. Nuesto programa
deberá ejecutar el programa pasado como primer argumento en foreground si no se
especifica la cadena “bg” y en background en caso contrario. Si el programa
tiene argumentos hay que ejecutarlo con éstos.
Compilación y enlazado: gcc ejer7.c -o ejer7
Ejecución: ./ejer7 nombre_programa [argumentos]
Autora: Elena Merelo Molina
*/

#include <unistd.h> //Para usar la familia de funciones exec()
#include <string.h> //Para usar strcmp
#include <stdio.h>  //Para usar la familia de funciones printf()
#include <stdlib.h> //Para exit()

int main(int argc, char *argv[]){
  int i, j= 0, status, child_process, exec_foreground= 1; //Partimos de que el programa se ejecuta en el foreground

  if(argc < 2){
    fprintf(stderr, "%s\n", "Como mínimo ha de introducir el nombre del programa");
    exit(EXIT_FAILURE);
  }

  /*Vamos recorriendo los argumentos del programa, y si alguno de ellos es la cadena "bg"
  ponemos exec_foreground a false, ya que se ejecutará el programa pasado como primer
  argumento en background.*/
  for(i= 1; i< argc && exec_foreground; i++){
    if(strcmp(argv[i], "bg") == 0)
      exec_foreground= 0;
  }
  
  if((child_process= fork()) == 0){ //Parte ejecutada por el hijo
    //Creamos un vector equivalente a char *argv[] pero sin el argumento bg ni argv[0]
    //char *argv_2[argc+1];
    for(i= 0; i< argc-1; i++){
      if(strcmp(argv[i], "bg") != 0)
        argv_2[j]= argv[i+1];

      j++;
    }
    /*Condición necesaria para que funcionen las funciones exec(), que el vector
    con los argumentos del programa acabe con NULL y empiece con el archivo que
    va a ser ejecutado.*/
    argv_2[j]= NULL;

    if(execv(argv[1], argv_2) == -1){
      fprintf(stderr, "%s\n", "Error %d en execv");
      exit(EXIT_FAILURE);
    }
  }
  //Part ejecutada por el padre
  else{
    printf("Soy el proceso padre y mi pid es: %d\n", getpid());

    /*Si el último argumento no es *bg*, se ejecuta en primer plano, por lo
    que el padre se espera al hijo.*/
    if(exec_foreground)
      waitpid(child_process,&status,0);
  }
}
