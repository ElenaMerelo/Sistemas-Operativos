## Sesión 5. Llamadas al sistema para gestión y control de señales.
### Actividad 5.1. Trabajo con las llamadas al sistema sigaction y kill.
A continuación se muestra el código fuente de dos programas. El programa send_signal permite el envío de una señal a un proceso identificado por medio de su PID. El programa get_signal se ejecuta en background y permite la recepción de señales.
**Ejercicio 1.** Compila y ejecuta los siguientes programas y trata de entender su funcionamiento.
~~~c
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
~~~
**Con este programa, dependiendo del número introducido por el usuario se mandan unas señales u otras a proceso, grupo de procesos,..., según sea pid.**

~~~c
/*
get_signal.c
Trabajo con llamadas al sistema del Subsistema de Procesos conforme a POSIX 2.10
Utilización de la llamada sigaction para cambiar el comportamiento del proceso
frente a la recepción de una señal.
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

//Determina cuál ha sido la señal recibida
static void sig_USR_hdlr(int sigNum){
  if(sigNum == SIGUSR1)
    printf("\nRecibida la señal SIGUSR1\n\n");
  else if(sigNum == SIGUSR2)
    printf("\nRecibida la señal SIGUSR2\n\n");
}

int main(int argc, char *argv[]){
  struct sigaction sig_USR_nact;

  //Para que escriba en la salida estándar sin buffering
  if(setvbuf(stdout,NULL,_ IONBF,0))
    perror("\nError en setvbuf");

  //Inicializamos la estructura sig_USR_na para especificar la nueva acción para la señal.
  sig_USR_nact.sa_handler= sig_USR_hdlr;

  //'sigemptyset' inicia el conjunto de señales dado al conjunto vacío.
  sigemptyset (&sig_USR_nact.sa_mask);
  sig_USR_nact.sa_flags = 0;
  //Establecer mi manejador particular de señal para SIGUSR1
  if( sigaction(SIGUSR1,&sig_USR_nact,NULL) <0){
    perror("\nError al intentar establecer el manejador de señal para SIGUSR1");
    exit(-1);
  }

  //Establecer mi manejador particular de señal para SIGUSR2
  if( sigaction(SIGUSR2,&sig_USR_nact,NULL) <0){
    perror("\nError al intentar establecer el manejador de señal para SIGUSR2");
    exit(-1);
  }
  for(;;){ }  //equivalente a while(1)
}
~~~
**Este segundo programa recibe la señal mandada por send_signal, al ejecutar de la siguiente manera:
+ Abrimos dos terminales en la carpeta donde tengamos ambos programas.
+ Los compilamos y enlazamos (`gcc send_signal.c -o send_signal` y `gcc get_signal.c -o get_signal`).
+ En uno de los terminales ponemos a ejecutar get_signal(`./get_signal`). Se quedará en bucle infinito de comprobación hasta que reciba una señal.
+ Desde el terminal restante escribimos `ps -ef | grep get_signal`, para saber el PID del programa, y ejecutamos `./send_signal [0, 1 ó 2] PID_obtenido`, recibiendo ya una señal get_signal. No finalizará get_signal hasta que ejecutemos con la opción 0.**

**Ejercicio 2.** Escribe un programa en C llamado contador, tal que cada vez que reciba una señal que se pueda manejar, muestre por pantalla la señal y el número de veces que se ha recibido ese tipo de señal, y un mensaje inicial indicando las señales que no puede manejar. En el cuadro siguiente se muestra un ejemplo de ejecución del programa.
~~~c

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*Vector en el que iremos almacenando las veces que aparece una señal. Le ponemos
ese tamaño ya que hay 27 señales, desde la 1 a la 27.*/
int number_of_times[28]= {0};

static void signal_handler(int signum){
  number_of_times[signum]++;
  printf("\nLa señal %d se ha recibido %d veces\n", signum, number_of_times[signum]);
  signal(signum, SIG_IGN);  //Ignora así la señal que recibe, solo cuenta el número de veces que se ha invocado
}

int main(int argc, char *argv[]){

  //Informamos de las señales que no podemos manejar
  printf("\nNo puedo manejar la señal %d", SIGKILL);
  printf("\nNo puedo manejar la señal %d", SIGSTOP);
  printf("\nEsperando el envío de señales...\n");

  //Para que escriba en la salida estándar sin buffering
  if(setvbuf(stdout,NULL,_ IONBF,0))
    perror("\nError en setvbuf");

  for(int i= 0; i< 28; i++){
    if(signal(i, signal_handler) < 0){
      printf("\nNo puedo manejar la señal %d\n", i);
      exit(EXIT_FAILURE);
    }
  }
  while(1); //Para que siga ejecutándose hasta que reciba una señal que termine el proceso
}
~~~
**Parece que funciona más o menos bien, pero no termina nunca, se queda siempre en bucle infinito y no he encontrado ningna versión de este programa resuelto que lo solucione. Para terminar el proceso, abrir otra terminal, y buscar el PID del proceso con `ps -ef|grep contador`, siendo contador el nombre de mi ejecutable, y luego escribiendo `kill -p *PID*` matamos al proceso, sino el procesador empieza a sobrecalentarse.**

### Actividad 5.2. Trabajo con las llamadas al sistema sigsuspend y sigprocmask

**Ejercicio 3.** Escribe un programa que suspenda la ejecución del proceso actual hasta que se reciba la señal SIGUSR1.
~~~c
//Escribe un programa que suspenda la ejecución del proceso actual hasta que se reciba la señal SIGUSR1.

#include <signal.h>
#include <stdio.h>

int main(){
  sigset_t new_mask;

  //Inicializamos la nueva máscara de señales a todas las posibles
  sigfillset(&new_mask);

  //Eliminamos del conjunto la señal SIGUSR1
  sigdelset(&new_mask, SIGUSR1);

  /*Espera a la señal SIGUSR1, ya que realmente está esperando a todas las señales
  excepto las del set, que son todas menos SIGUSR1.*/
  sigsuspend(&new_mask);

}
~~~



### Extra:
En el siguiente ejemplo se suspende la ejecución del proceso actual hasta que reciba una señal distinta de SIGUSR1.
~~~c
//tarea11.c
#include <stdio.h>
#include <signal.h>
int main(){
  sigset_t new_mask;
  /*inicializar la nueva mascara de señales*/
  sigemptyset(&new_mask);
  sigaddset(&new_mask, SIGUSR1);
  /*esperar a cualquier señal excepto SIGUSR1*/
  sigsuspend(&new_mask);
}
~~~
**sigwait(3)**
~~~
The  sigwait()  function suspends execution of the calling thread until
       one of the signals specified in the signal  set  set  becomes  pending.
       The  function  accepts  the signal (removes it from the pending list of
       signals), and returns the signal number in sig.

       The operation of sigwait() is the same as sigwaitinfo(2), except that:

       * sigwait() returns only the signal number,  rather  than  a  siginfo_t
         structure describing the signal.

       * The return values of the two functions are different.
~~~
**sigwaitinfo(2)**
~~~
#include <signal.h>

       int sigwaitinfo(const sigset_t *set, siginfo_t *info);
sigwaitinfo() suspends execution of the calling thread until one of the signals  in  set  is  pending  (If one of the signals in set is already pending for the calling thread, sigwaitinfo() will return immediately.)

sigwaitinfo() removes the signal from the set of  pending  signals  and returns the signal number as its function result.  If the info argument is not NULL, then the buffer that it points to  is  used  to  return  a structure  of  type siginfo_t (see sigaction(2)) containing information about the signal.

If multiple signals in set are pending for the caller, the signal  that is  retrieved  by  sigwaitinfo()  is  determined according to the usual ordering rules; see signal(7) for further details.

siginfo_t {
               int      si_signo;     /* Signal number */
               int      si_errno;     /* An errno value */
               int      si_code;      /* Signal code */
               int      si_trapno;    /* Trap number that caused
                                         hardware-generated signal
                                         (unused on most architectures) */
               pid_t    si_pid;       /* Sending process ID */
               uid_t    si_uid;       /* Real user ID of sending process */
               int      si_status;    /* Exit value or signal */
               clock_t  si_utime;     /* User time consumed */
               clock_t  si_stime;     /* System time consumed */
               sigval_t si_value;     /* Signal value */
               int      si_int;       /* POSIX.1b signal */
On success, sigwaitinfo() returns a signal  number. On failure both it returns -1, with errno set to indicate the error.

RETURN VALUE
       sigaction() returns 0 on success; on error, -1 is returned,  and  errno
       is set to indicate the error.

ERRORS
       EFAULT act  or oldact points to memory which is not a valid part of the
              process address space.

       EINVAL An invalid signal was specified.  This will also be generated if an  attempt is made to change the action for SIGKILL or SIGSTOP, which cannot be caught or ignored.

~~~
*If  SA_SIGINFO  is specified in sa_flags, then sa_sigaction (instead of
       sa_handler) specifies the signal-handling function  for  signum.   This
       function receives the signal number as its first argument, a pointer to
       a siginfo_t as its second argument and a pointer to a ucontext_t  (cast
       to  void *)  as  its  third  argument.  (Commonly, the handler function
       doesn't make any use of the third argument.  See getcontext(3) for fur‐
       ther information about ucontext_t.)*

> sigaction puede llamarse con un segundo argumento nulo para conocer el manejador de señal en curso. También puede emplearse para comprobar si una señal dada es válida para la máquina donde está, llamándola con el segundo y el tercer argumento nulos.

Los siguientes ejemplos ilustran el uso de la llamada al sistema sigaction para establecer un manejador para la señal SIGINT que se genera cuando se pulsa <CTRL+C>.
~~~c
// tarea9.c
#include <stdio.h>
#include <signal.h>
int main(){
  struct sigaction sa;
  sa.sa_handler = SIG_IGN; // ignora la señal
  sigemptyset(&sa.sa_mask);
  //Reiniciar las funciones que hayan sido interrumpidas por un manejador
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    printf("Error en el manejador");

  while(1);
}

// tarea10.c
#include <stdio.h>
#include <signal.h>
static int s_recibida=0;
static void handler (int signum){
  printf("\n Nueva acción del manejador \n");
  s_recibida++;
}

int main(){
  struct sigaction sa;
  sa.sa_handler = handler; // establece el manejador a handler
  sigemptyset(&sa.sa_mask);
  //Reiniciar las funciones que hayan sido interrumpidas por un manejador
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    printf("error en el manejador");
  while(s_recibida<3);
}
