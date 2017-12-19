## Resolución del examen de prácticas correspondientes al módulo 2, del curso 2013-2014
>Profesor: Jose Antonio Gómez Hernández

### Preguntas:

+ Parte obligatoria mínima (50%):
Construir un único programa que cree un proceso hijo. Ambos procesos utilizaran un cauce con/sin nombre para comunicarse (el tipo de cauce lo dejo a vuestra elección). Donde el trabajo de cada proceso es:

  + El padre: Lee el directorio actual y para cada archivo regular contenido en el mismo obtiene e imprime en pantalla su número de inodo junto con el UID del propietario, y escribe el nombre del archivo en el cauce.

  + El hijo: Lee del cauce el nombre del archivo escrito por el padre mediante el mecanismo de proyección de archivos. Tras lo cual, muestra en pantalla su contenido.

+ Ampliación:

  + [15%] El padre se asegura que los archivos cuyo nombre va a pasar al hijo tienen permiso de lectura para el proceso, si no fuese así, lo cambia para que los pueda leer el hijo.

  + [15%] Establecer un manejador para la señal SIGPIPE en el padre en caso de que el proceso hijo finalice inesperadamente de forma que se indique el evento por pantalla.

  + [20%] El acceso al archivo por parte del proceso hijo es exclusivo, es decir, bloqueará el archivo antes de acceder al mismo y lo desbloqueará al finalizar.


Construir el programa necesario para implementar dicha especificación realizando las suposiciones que estime necesarias y que sean lo menos restrictivas posibles.
~~~c
/*
Archivo: examen.c
Compilación y enlazado: gcc examen.c -o examen
Ejecución: ./examen
Versión usando un cauce sin nombre
Autora: Elena Merelo Molina
*/

#include <unistd.h> //pipe(), fork(), getuid(), familia de funciones execl(), stat()
#include <stdio.h>  //familia de funciones printf()
#include <stdlib.h> //exit()
#include <sys/types.h>  //getuid(), stat()
#include <dirent.h> //opendir(), readdir()
#include <string.h> //strlen()
#include <sys/stat.h> //familia de funciones stat()
#include <fcntl.h>  //Para las macros de chmod,...
#include <signal.h> //sigaction() y demases
#include <sys/mman.h> //mmap y sus macros/flags

static void signal_handler(int signum){
  if(signum == SIGPIPE)
    printf("\nBroken pipe: write to pipe with no readers\n");
}

int SIZE= 256;

int main(){
  int fd[2], num_bytes;
  pid_t child_process;
  char buffer[100];
  struct stat attributes, attributes_buffer;
  DIR* dir;
  struct dirent* dp;
  int filename;
  char* projection;

  //Creamos el pipe para la comunicación entre los procesos
  if(pipe(fd) == -1){
    fprintf(stderr, "\n%s\n", "Error en pipe");
    exit(EXIT_FAILURE);
  }

  //Creamos el proceso hijo
  if((child_process= fork())== -1){
    perror("\nError en fork\n");
    exit(EXIT_FAILURE);
  }
  /*Queremos que el proceso padre envíe datos al hijo, para lo que debe cerrar
  el descriptor usado para la lectura(fd[0]), y el hijo debe cerrar el descriptor
  usado para la escritura(fd[1])*/

  /*Parte ejecutada por el hijo. Lee del cauce el nombre del archivo escrito por
  el padre y muestra en pantalla su contenido, como haría la orden cat.*/
  if(child_process == 0){
    close(fd[1]);

    while((num_bytes= read(fd[0], buffer, SIZE)) != 0){
      if(num_bytes == -1){
        perror("\nError en la lectura");
        exit(EXIT_FAILURE);
      }
      //Cargamos los atributos de buffer en atributes_buffer
      if(stat(buffer, &attributes_buffer) < 0){
        printf("\nError al intentar acceder a los atributos de %s\n", buffer);
        perror("\nError en stat\n");
        exit(EXIT_FAILURE);
      }

      //Abrimos el archivo correspondiente
      if((filename= open(buffer, O_RDWR|O_EXCL, S_IRWXU)) < 0){
        printf("\nError al abrir el archivo %s\n", buffer);
        exit(EXIT_FAILURE);
      }

      //Leemos del cauce el nombre del archivo escrito por el padre mediante el mecanismo de proyección de archivos
      printf("(%lu\n", attributes_buffer.st_size);

      //El primer argumento es 0, luego el kernel decide la dirección en la que crear el mapping
      projection = (char * ) mmap (0, attributes_buffer.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, filename, 0);

      if (projection == MAP_FAILED) {
        perror("Falló la proyección");
        exit(EXIT_FAILURE);
      }
      printf("\nArchivo mostrado: %s\n%s\n", buffer, projection);
      close(filename);
    }
    printf("\n%s\n","Proceso hijo finalizado" );

    //Mostramos por pantalla el contenido del archivo proyectado por el proceso padre
    /*if(execl("cat", "cat", buffer, NULL) == -1)
      fprintf(stderr, "\nNo se puede mostrar el contenido de %s\n", buffer);*/

    exit(EXIT_SUCCESS);
  }
  else{ //Parte ejecutada por el proceso padre
    close(fd[0]);

    //Abrimos el flujo correspondiente al directorio actual
    dir= opendir(get_current_dir_name());

    while((dp= readdir(dir)) != NULL){  //Mientras haya archivos que leer
      //Obtenemos información del archivo dp->d_name y la almacenamos en atributes
      if(stat(dp->d_name, &attributes) < 0){
        printf("\nError al intentar acceder a los atributos de %s\n", dp->d_name);
        perror("\nError en stat\n");
        exit(EXIT_FAILURE);
      }

      //Nos cercioramos de que no sea un archivo de los ocultos
      if(dp->d_name[0] != '.' && dp->d_name[strlen(dp->d_name)-1] != '~'){
        if(S_ISREG(attributes.st_mode)){ //Si el archivo es regular
          printf("\nNúmero de inodo del archivo %s: %d, UID del propietario: %d", dp->d_name, dp->d_ino, getuid());
          //Escribimos el nombre del archivo en el cauce
          write(fd[1], dp->d_name, sizeof(dp->d_name));
        }
      }
      /*Nos aseguramos de que los archivos cuyo nombre va a pasar al hijo tienen permiso de lectura para el proceso, si no fuese así, los cambiamos para que los pueda leer el hijo.*/
      if((attributes.st_mode & S_IRGRP) != S_IRGRP){
        int new_mode= attributes.st_mode;
        new_mode |= S_IRGRP;
        chmod(dp->d_name, new_mode);
      }
    }
    /*Establecemos un manejador para la señal SIGPIPE en el padre en caso de que el proceso hijo finalice inesperadamente, de forma que se indique el evento por pantalla.*/
    struct sigaction sig_act;

    //Para que escriba en la salida estándar sin buffering
    if(setvbuf(stdout, NULL, _ IONBF,0))
      perror("\nError en setvbuf");

    //Inicializamos la estructura sig_act para especificar la nueva acción para la señal
    sig_act.sa_handler= signal_handler;

    //Con 'sigemptyset' inicializamos el conjunto de señales dado al conjunto vacío
    sigemptyset (&sig_act.sa_mask);
    sig_act.sa_flags = 0;

    //Establecemos un manejador particular de señal para SIGPIPE
    if(sigaction(SIGPIPE,&sig_act,NULL) <0){
      perror("\nError al intentar establecer el manejador de señal para SIGPIPE");
      exit(-1);
    }
    exit(EXIT_SUCCESS);
  }
  exit(EXIT_SUCCESS);
}

~~~
