## Sesión 3. Llamadas al sistema para el Control de Procesos
### Actividad 3.1 Trabajo con la llamada al sistema fork
**Ejercicio 1.**  Implementa un programa en C que tenga como argumento un número entero. Este programa debe crear un proceso hijo que se encargará de comprobar si dicho número es un número par o impar e informará al usuario con un mensaje que se enviará por la salida estándar. A su vez, el proceso padre comprobará si dicho número es divisible por 4, e informará si lo es o no usando igualmente la salida estándar.
~~~c
/*
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
~~~
**Ejercicio 2.** ¿Qué hace el siguiente programa? Intenta entender lo que ocurre con las variables y sobre todo con los mensajes por pantalla cuando el núcleo tiene activado/desactivado el mecanismo de buffering.
~~~c
/*
tarea4.c
Trabajo con llamadas al sistema de Control de Procesos "POSIX 2.10 compliant"
Prueba el programa tal y como está. Después, elimina los comentarios (1) y
pruébalo de nuevo.
*/
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>

int global=6;
char buf[]="\nPrueba usando fork()";

int main(int argc, char *argv[]){
  int var;
  pid_t pid;
  var=88;
  if(write(STDOUT_FILENO, buf, sizeof(buf)+1) != sizeof(buf)+1) {
    perror("\nError en write");
    exit(-1);
  }
  /*Esta llamada con  _IONBF hace que la información aparezca en el archivo destino, en este caso
  stdout, tan pronto como se escribe, que el flujo de salida sea "unbuffered". Se usa pues para cambiar el buffer de un flujo de salida.*/
  if(setvbuf(stdout, NULL, `_IONBF, 0)){  // La comilla invertida se pone para que aparezca bien en markdown, quitarla para compilar el programa
      perror("\nError en setvbuf");
  }

  printf("\nMensaje previo a la ejecución de fork");
  if((pid= fork())< 0){
    perror("\nError en el fork");
    exit(-1);
  }
  else if(pid==0){
    //proceso hijo ejecutando el programa
    global++;
    var++;
  }
  else //proceso padre ejecutando el programa
    sleep(1); //Importante! Detecta el fin del proceso hijo y continúa. Si no se pone los valores del padre no se modificarán o no aparecerán, no funcionará correctamente
  //Mostramos por pantalla el pid del proceso padre y los valores global y variable. Tendrán
  //un valor distinto a los del proceso padre al haber sido modificados una vez hecho el fork(), y
  //hace dicha Modificación solo para el proceso hijo
  printf("\npid= %d, global= %d, var= %d\n", getpid(),global,var);
  exit(0);
}

~~~
*Nota 1*: El núcleo no realiza buffering de salida con la llamada al sistema `write`. Esto quiere
decir que cuando usamos `write(STDOUT_FILENO,buf,size)`, los datos se escriben directamente
en la salida estándar sin ser almacenados en un buffer temporal. Sin embargo, el núcleo sí realiza buffering de salida en las funciones de la biblioteca estándar de E/S del C, en la cual está incluida `printf`. Para deshabilitar el buffering en la biblioteca estándar de E/S se utiliza la función:
`int setvbuf(FILE *stream, char *buf, int mode , size_t size);`
*Nota 2* : En la parte de llamadas al sistema para el sistema de archivos vimos que en Linux se
definen las macros `STDIN_FILENO`, `STDOUT_FILENO` y `STDERR_FILENO` para poder utilizar las
llamadas al sistema `read` y `write` (que trabajan con descriptores de archivo) sobre la entrada
estándar, la salida estándar y el error estándar del proceso. Además, en <stdio.h> se definen
tres flujos (STREAM) para poder trabajar sobre estos archivos especiales usando las funciones de
la biblioteca de E/S del C: `stdin`, `stdout` y `stderr`:
~~~c
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
~~~
¡Fíjate que setvbuf es una función que trabaja sobre STREAMS, no sobre descriptores de
archivo!

**Ejercicio 3.** Indica qué tipo de jerarquías de procesos se generan mediante la ejecución de cada
uno de los siguientes fragmentos de código. Comprueba tu solución implementando un código para generar 20 procesos en cada caso, en donde cada proceso imprima su PID y el del padre.*Fragmentos de código proporcionados:*
~~~c
/*
Jerarquía de procesos tipo 1
*/
for (i=1; i < nprocs; i++) {
  if ((childpid= fork()) == -1) {
    fprintf(stderr, "Could not create child %d: %s\n",i,strerror(errno));
    exit(-1);
  }
}
if (childpid) //Ésto está mal. El break ha de estar dentro del for, si no, no hay nada de lo que salir, da error al compilar.
  break;
/*
Jerarquía de procesos tipo 2
*/
for (i=1; i < nprocs; i++) {
  if ((childpid= fork()) == -1) {
    fprintf(stderr, "Could not create child %d: %s\n",i,strerror(errno));
    exit(-1);
  }
}
if (!childpid)  //Same as before
  break;
~~~
**Mi versión, comentada: **
~~~c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(){
  pid_t childpid;
  int nprocs= 20;
  /*Jerarquía de procesos tipo 1. Crea 20 procesos hijos de forma recursiva y cuando terminan
  se sale del bucle, cuando le toca ejecutar al padre se finaliza. Cada hijo es hijo del proceso
  creado anteriormente, se van descendiendo niveles en el árbol con raíz el proceso padre(padre->hijo->hijo...)*/

  fprintf(stdout, "\n%s\n", "Jerarquía de procesos 1");
  for (int i= 1; i < nprocs; i++) {
    if ((childpid= fork()) == -1) {
      fprintf(stderr, "\nCould not create child %d: %s", i, strerror(errno));
      exit(EXIT_FAILURE);
    }

    if(childpid== 0) //Si se crea el proceso hijo, ejecutará:
      printf("\nChild process %d created. PID: %d, PPID: %d\n", i+1, getpid(), getppid());

    if (childpid){  //Parte ejecutada por el proceso padre
      wait(NULL); //Espera a que los procesos hijos cambien su estado, ya sea porque han finalizado o obligados a finalizar
      break;//exit(EXIT_FAILURE);
    }
  }
  /*Jerarquía de procesos tipo 2. Crea en el mismo nivel hijos de un proceso, cuando
  está ejecutando el proceso hijo no hace nada, pero cuando llega al padre itera
  para crear más hijos.*/

  pid_t childpid_2;

  for (int i= 1; i < nprocs; i++) {
    if ((childpid_2= fork()) == -1) {
      fprintf(stderr, "Could not create child %d: %s\n",i,strerror(errno));
      exit(-1);
    }
    /*En el guión no incluía esta llamada, pero la ponemos para que cada proceso
    tenga asignado un PID y PPID correcto, les de tiempo a terminar de crearse.*/
    wait(NULL);
    if (!childpid_2)
      //Esta parte se ejecuta por el proceso hijo
      printf("\nChild process %d created. PID: %d, PPID: %d\n", i+1, getpid(), getppid());
      break;
  }
}
~~~
### Actividad 3.2 Trabajo con las llamadas al sistema `wait`, `waitpid` y `exit`
**Ejercicio 4.** Implementa un programa que lance cinco procesos hijo. Cada uno de ellos se
identificará en la salida estándar, mostrando un mensaje del tipo Soy el hijo PID. El proceso
Guía Práctica de Sistemas Operativos-97padre simplemente tendrá que esperar la finalización de todos sus hijos y cada vez que detecte
la finalización de uno de sus hijos escribirá en la salida estándar un mensaje del tipo:
Acaba de finalizar mi hijo con <PID>
Sólo me quedan <NUM_HIJOS> hijos vivos


### Extra
No olvidar:
~~~c
pid = fork(); /* call fork() from parent process*/
if (0 == pid) {
  /* fork returned 0. This part will be executed by child process*/
  /*  getpid() will give child process id here*/
}
else {
  /* fork returned child pid which is non zero. This part will be executed by parent process*/
  /*  getpid() will give parent process id here */
}
~~~

#
