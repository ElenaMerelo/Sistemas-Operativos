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
** Mi versión, comentada: **
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
**Ejercicio 4.** Implementa un programa que lance cinco procesos hijo. Cada uno de ellos se identificará en la salida estándar, mostrando un mensaje del tipo Soy el hijo PID. El proceso padre simplemente tendrá que esperar la finalización de todos sus hijos y cada vez que detecte la finalización de uno de sus hijos escribirá en la salida estándar un mensaje del tipo:
Acaba de finalizar mi hijo con <PID>
Sólo me quedan <NUM_HIJOS> hijos vivos
+ Forma 1: **Conforme vamos creando los procesos se van matando, de manera que solo hay un proceso hijo vivo cada vez.**

~~~c
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
  int num_processes= 5, status;
  pid_t child_process;

  for(int i= 0; i< num_processes; i++){
    if((child_process= fork()) == -1){  //Si no se crea correctamente el hijo imprimimos el correspondiente mensaje de error
      fprintf(stderr, "\n%s\n", "Error en fork" );
      exit(EXIT_FAILURE);
    }
    if(child_process == 0){ //Si se ha creado correctamente
      //Parte ejecutada por el proceso hijo
      fprintf(stdout, "\nSoy el hijo %d con PID %d y PPID %d\n", i, getpid(), getppid());
      exit(0);
    }

    else{
      //Parte ejecutada por el proceso padre
      child_process = wait(&status);
      fprintf(stdout, "\nAcaba de finalizar mi hijo con PID %d\n", child_process);
    }
  }
}
~~~
+ Forma 2: **Se crean cinco hijos, iteramos cinco veces sobre el wait y cada vez que finaliza uno se informa del PID del hijo que ha terminado y de los hijos que quedan vivos.**

~~~c
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
  int num_processes= 5, status;
  pid_t child_process;

  for(int i= 0; i< num_processes; i++){
    if((child_process= fork()) == -1){  //Si no se crea correctamente el hijo imprimimos el correspondiente mensaje de error
      fprintf(stderr, "\n%s\n", "Error en fork" );
      exit(EXIT_FAILURE);
    }
    if(child_process == 0){ //Si se ha creado correctamente
      //Parte ejecutada por el proceso hijo
      fprintf(stdout, "\nSoy el hijo %d con PID %d y PPID %d\n", i, getpid(), getppid());
      exit(0);
    }
  }
  /*Como hemos creado 5 hijos, iteramos 5 veces sobre wait, cada vez que se detecta que ha acabado uno,
  informa del PID del hijo que ha acabado e informa de los hijos que quedan vivos. No hace
  falta comprobar dentro del for if(child_process > 0) al haber puesto exit(0) al crear los procesos hijos.*/
  for(int i= 4; i>= 0; i--){
    child_process= wait(&status);
    printf("\nHa finalizado mi hijo con PID %d" , child_process);
    printf("\nMe quedan %d hijos vivos",i);
  }
}
~~~
**Ejercicio 5.** Implementa una modificación sobre el anterior programa en la que el proceso padre espera primero a los hijos creados en orden impar (1º,3º,5º) y después a los hijos pares (2º
y 4º).
~~~c
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
  int num_processes= 5, status, children[num_processes];  //En éste último vamos almacenando los PIDs de los procesos hijos
  //pid_t child_process;

  for(int i= 0; i< num_processes; i++){
    if((children[i]= fork()) == -1){  //Si no se crea correctamente el hijo imprimimos el correspondiente mensaje de error
      fprintf(stderr, "\n%s\n", "Error en fork" );
      exit(EXIT_FAILURE);
    }
    if(children[i] == 0){ //Si se ha creado correctamente
      //Parte ejecutada por el proceso hijo
      fprintf(stdout, "\nSoy el hijo %d con PID %d y PPID %d\n", i, getpid(), getppid());
      exit(0);
    }
  }
  /*Como hemos creado 5 hijos, iteramos 5 veces sobre wait, cada vez que se detecta que ha acabado uno,
  informa del PID del hijo que ha acabado e informa de los hijos que quedan vivos. No hace
  falta comprobar dentro del for if(child_process > 0) al haber puesto exit(0) al crear los procesos hijos.*/
  //Esperamos a los hijos impares, que se corresponden con las posiciones pares del vector
  for(int i= 0; i< 5; i++){
    if(i%2 == 0){
      waitpid(children[i], &status, 0);
      printf("\nHa finalizado mi hijo con PID %d y estado %d\n", children[i], status);
      printf("\nMe quedan %d hijos vivos, éste es el hijo %d\n", --num_processes, i+1);
    }
  }
  for(int i= 0; i< 5; i++){
    //Esperamos a los hijos pares, que se corresponden con las posiciones impares del vector
    if(i%2 != 0){
      waitpid(children[i], &status, 0);
      printf("\nHa finalizado mi hijo con PID %d y estado %d\n", children[i], status);
      printf("\nMe quedan %d hijos vivos, éste es el hijo %d\n", --num_processes, i+1);
    }
  }
}
~~~
### Actividad 3.3 Trabajo con la familia de llamadas al sistema exec
**Ejercicio 6.** ¿Qué hace el siguiente programa? **En éste programa es el proceso hijo el que ejecuta el programa, imprimiendo con ldd las bibliotecas dinámicas requeridas por cada programa pasado como argumento, en este caso el propio. Lo vemos comentado:**
~~~c
/*
tarea5.c
Trabajo con llamadas al sistema del Subsistema de Procesos conforme a POSIX 2.10
*/
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
int main(int argc, char *argv[]){
  pid_t pid;
  int estado;

  if( (pid=fork())<0) {
    perror("\nError en el fork");
    exit(-1);
  }
  else if(pid==0) { //proceso hijo ejecutando el programa
    //Ejecuta desde /usr/bin/ldd el programa tarea5, mostrando con ldd las bibliotecas dinámicas que precisa
    if((execl("/usr/bin/ldd","ldd","./tarea5",NULL)<0)){
      perror("\nError en el execl");
      exit(-1);
    }
  }
  //El proceso padre espera a que el estado del hijo cambie
  wait(&estado);

  /*<estado> mantiene información codificada a nivel de bit sobre el motivo de
  finalización del proceso hijo que puede ser el número de señal o 0 si alcanzó
  su finalización normalmente. Mediante la variable estado de wait(), el proceso
  padre recupera el valor especificado por el proceso hijo como argumento de la
  llamada exit(), pero desplazado 1 byte porque el sistema incluye en el byte
  menos significativo el código de la señal que puede estar asociada a la terminación
  del hijo. Por eso se utiliza estado>>8, de forma que obtenemos el valor del argumento
  de exit() del hijo.*/
  printf("\nMi hijo %d ha finalizado con el estado %d\n", pid, estado>>8);
  exit(0);
}
~~~
**Ejercicio 7.** Escribe un programa que acepte como argumentos el nombre de un programa, sus argumentos si los tiene, y opcionalmente la cadena “bg”. Nuesto programa deberá ejecutar el programa pasado como primer argumento en foreground si no se especifica la cadena “bg” y en background en caso contrario. Si el programa tiene argumentos hay que ejecutarlo con éstos.
~~~c
//File: ejer7.c
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
    //Creamos un vector equivalente a char* argv[] pero sin el argumento bg ni argv[0]
    char* argv_2[argc+1];
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
    que el padre se espera al hijo.* /
    if(exec_foreground)
      waitpid(child_process,&status,0);
  }
}
~~~
**Otra forma de hacer el programa, suponiendo que bg se pone al final sería la siguiente. La anterior forma no siempre funciona, pero ésta sí**
~~~c
//File: ejer_2.c
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
  int num_arguments= argc;

  if(argc < 2){
    fprintf(stderr, "%s\n", "Como mínimo ha de introducir el nombre del programa");
    exit(EXIT_FAILURE);
  }

  /*Si el último argumento es la cadena "bg" ponemos exec_foreground a false,
  ya que se ejecutará el programa pasado como primer argumento en background.*/
  if(strcmp(argv[argc-1], "bg") == 0){
    num_arguments= argc-1;
    exec_foreground= 0;
  }

  if((child_process= fork()) == 0){ //Parte ejecutada por el hijo
    //Creamos un vector equivalente a char *argv[] pero sin el argumento bg ni argv[0]
    char* argv_2[num_arguments];
    for(i= 0; i< num_arguments-1; i++)
        argv_2[i]= argv[i+1];
    argv_2[num_arguments-1]= NULL;

    if(execv(argv[1], argv_2) == -1){
      fprintf(stderr, "%s\n", "Error %d en execv");
      exit(EXIT_FAILURE);
    }
  }
  //Part ejecutada por el padre
  else{
    printf("Soy el proceso padre y mi pid es: %d\n", getpid());

    /*Si el último argumento no es *bg*, se ejecuta en primer plano, por lo
    que el padre se espera al hijo.* /
    if(exec_foreground)
      waitpid(child_process,&status,0);
  }
}
~~~
### Extra
> No olvidar:
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

##### waitid()
~~~
#include <sys/types.h>
#include <sys/wait.h>
int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
waitid()
     The  waitid()  system  call (available since Linux 2.6.9) provides more
     precise control over which child state changes to wait for.

     The idtype and id arguments select the child(ren) to wait for, as  follows:

     idtype == P_PID
            Wait for the child whose process ID matches id.

     idtype == P_PGID
            Wait for any child whose process group ID matches id.

     idtype == P_ALL
            Wait for any child; id is ignored.

     The  child state changes to wait for are specified by ORing one or more
     of the following flags in options:

     WEXITED     Wait for children that have terminated.

     WSTOPPED    Wait for children that have been stopped by delivery  of  a
                 signal.

     WCONTINUED  Wait  for  (previously  stopped)  children  that  have been
                 resumed by delivery of SIGCONT.

     The following flags may additionally be ORed in options:

     WNOHANG     As for waitpid().

     WNOWAIT     Leave the child in a waitable state; a later wait call  can
                 be used to again retrieve the child status information.

Upon  successful  return, waitid() fills in the following fields of the
     siginfo_t structure pointed to by infop:

     si_pid      The process ID of the child.

     si_uid      The real user ID of the child.  (This field is not  set  on
                 most other implementations.)

     si_signo    Always set to SIGCHLD.

     si_status   Either  the  exit status of the child, as given to _exit(2)
                 (or exit(3)), or the signal that caused the child to termi‐
                 nate,  stop, or continue.  The si_code field can be used to
                 determine how to interpret this field.

     si_code     Set  to  one  of:  CLD_EXITED  (child   called   _exit(2));
                 CLD_KILLED  (child  killed  by  signal);  CLD_DUMPED (child
                 killed by signal,  and  dumped  core);  CLD_STOPPED  (child
                 stopped by signal); CLD_TRAPPED (traced child has trapped);
                 or CLD_CONTINUED (child continued by SIGCONT).
                 waitid():  returns  0  on  success  or  if WNOHANG was specified and no
     child(ren) specified by id has yet  changed  state;  on  error,  -1  is
     returned.

     Each  of  these calls sets errno to an appropriate value in the case of
     an error.
~~~
>La orden wait espera a que finalice un hijo. Cuando un hijo termina, se almacena en la variable estado que ha terminado, devoldiendo wait el PID del proceso que ha terminado, para almacenarlo en PID. La orden wait espera a que termine un hijo, mientras no termina, actualizo su estado en la variable estado. Si queremos esperar a un hijo concreto, debemos usar la orden waitpid, que nos permite seleccionar el hijo al que queremos esperar. El uso de wait es equivalente a declarar waitpid de la siguiente manera:
`waitpid(-1, &estado, 0)`
Por defecto, waitpid espera a que termine el proceso, especificado mediante su PID, dado en el primer argumento, aunque esto puede modificarse mediante opciones especificadas en el tercer argumento.


**Crear un hilo:** Para ello usamos `clone()` con los siguientes indicadores:
`CLONE_VM|CLONE_FILES|CLONE_FS|CLONE_THREAD|CLONE_SIGHAND`. Con ello, el hijo comparte con su padre la memoria virtual, los archivos abiertos, el directorio raíz, el directorio de trabajo y la máscara de creación de archivos, pone al hijo en el mismo grupo del padre, y comparten los manejadores de señales.
#
