#Sesión 1. Llamadas al sistema para el Sistema de archivos (Parte I)
**Ejercicio 1.** ¿Qué hace el siguiente programa? Probad tras la ejecución del programa las siguientes órdenes del shell: `cat archivo` y `od -c archivo`
~~~c
/*
tarea1.c
Trabajo con llamadas al sistema del Sistema de Archivos ''POSIX 2.10 compliant''
*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
char buf1[]="abcdefghij";
char buf2[]="ABCDEFGHIJ";
int main(int argc, char *argv[])
  int fd;

  /*Crea un archivo en modo solo escritura, y S_IRUSR indica que el usuario
  tiene permiso de lectura, S_IWUSR que el usuario tiene permisos de escritura.
  Si la llamada al sistema open devuelve un valor negativo es que ha fallado. */
  if( (fd=open("archivo",O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR))<0) {  
    printf("\nError %d en open",errno);
    perror("\nError en open");
    exit(-1);
  }

  /*Notificamos cuando se escriban en el archivo un número distinto a 10 bytes, que es
  el tamaño del bf1. Esto puede ocurrir por ejemplo si no hay suficiente memoria.
  Esta llamada escribe los 10 bytes del buffer apuntado por buf1 en el fichero referido.*/
  if(write(fd,buf1,10) != 10) {
    perror("\nError en primer write");
    exit(-1);
  }

  /* Se pone el offset a 40 bytes. Si funciona correctamente devuelve la localización
  del offset en bytes contada desde el principio del archivo. Si se ha producido un error
  se devuelve -1.*/
  if(lseek(fd,40,SEEK_SET) < 0) {
    perror("\nError en lseek");
    exit(-1);
  }

  /* Notificamos cuando se escriben un número distinto de 10 bytes, significará que algo
  ha fallado ya que write() devuelve el número de bytes escritos en fd.*/
  if(write(fd,buf2,10) != 10) {
    perror("\nError en segundo write");
    exit(-1);
  }
  close(fd);
  return 0;
}
~~~
**Una vez ejecutado el programa probamos las órdenes en el shell:**
~~~shell
$ cat archivo
abcdefghijABCDEFGHIJ

$ od -c archivo
0000000   a   b   c   d   e   f   g   h   i   j  \0  \0  \0  \0  \0  \0
0000020  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
0000040  \0  \0  \0  \0  \0  \0  \0  \0   A   B   C   D   E   F   G   H
0000060   I   J
0000062
~~~
**Como vemos se han escrito correctamente en el archivo los 10 bytes del buf1,
se ha aplicado el offset de 40 bytes y se han escrito los otros 10 bytes del buf2.**

**Ejercicio 2.** Implementa un programa que realice la siguiente funcionalidad. El programa acepta como argumento el nombre de un archivo (pathname), lo abre y lo lee en bloques de tamaño 80 Bytes, y crea un nuevo archivo de salida, salida.txt, en el que debe aparecer la siguiente información:
Bloque 1
// Aquí van los primeros 80 Bytes del archivo pasado como argumento.
Bloque 2
// Aquí van los siguientes 80 Bytes del archivo pasado como argumento.
...
Bloque n
// Aquí van los siguientes 80 Bytes del archivo pasado como argumento.
Si no se pasa un argumento al programa se debe utilizar la entrada estándar como archivo de entrada.
*Modificación adicional.* ¿Cómo tendrías que modificar el programa para que una vez finalizada la escritura en el archivo de salida y antes de cerrarlo, pudiésemos indicar en su primera línea el número de etiquetas ''Bloque i'' escritas de forma que tuviese la siguiente apariencia?:
El número de bloques es <no_bloques>
Bloque 1
// Aquí van los primeros 80 Bytes del archivo pasado como argumento.
Bloque 2
// Aquí van los siguientes 80 Bytes del archivo pasado como argumento.
...
~~~c
//File: Ejercicio2-Módulo2.c
//Authors: Elena Merelo Molina y Antonio Gámiz Delgado
//Compilation: gcc Ejercicio2-Módulo2.c -o Ejercicio2-Módulo2
//Execution: ./Ejercicio2-Módulo2 prueba.txt. It generates salida.txt

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>

char buffer1[80];

int main(int argc, char *argv[]){
  int fd, fd_o, i=0;
  if(argc != 2)
    fd=0;
  else{
    if((fd=open(argv[1],O_RDONLY,S_IRUSR|S_IWUSR))<0) {
      printf("\nError %d en open",errno);
      perror("\nError en open");
      exit(-1);
    }
  }

  if((fd_o=open("salida.txt",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))<0) {
    printf("\nError %d en open",errno);
    perror("\nError en open");
    exit(-1);
  }

  struct stat atributos;
  if(lstat(argv[1], &atributos) < 0)
    printf("%s\n", "Error en lstat" );

  long long int n= atributos.st_size/80;
  char buffer2[]="El número de bloques es: ";
  dprintf(fd_o, "%s : %lld", buffer2, n);

  char buffer3[]="\nBloque";

  while(read(fd, buffer1, 80) != 0){
    dprintf(fd_o, "%s %d : %s", buffer3, i++, "\n");
    if(write(fd_o, buffer1, 80) != 80) {
      perror("\nError write");
      exit(-1);
    }

  }
  close(fd);
  close(fd_o);
  return 0;
}
~~~
#####Actividad 3.2. Trabajo con llamadas al sistema de la familia stat.
Consulta las llamadas al sistema stat y lstat para entender sus diferencias.
**La única diferencia entre  `int stat(const char *pathname, struct stat *buf)` e `int lstat(const char *pathname, struct stat *buf)` es que en lstat si se pasa como pathname un enlace simbólico devuelve información sobre el enlace en sí, no sobre el fichero al que se refiere.**
**Ejercicio 3.** ¿Qué hace el siguiente programa?
~~~c
/*
tarea2.c
Trabajo con llamadas al sistema del Sistema de Archivos ''POSIX 2.10 compliant''
*/
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
int main(int argc, char *argv[]){
  int i;
  struct stat atributos;
  char tipoArchivo[30];
  if(argc<2) {
    printf("\nSintaxis de ejecucion: tarea2 [<nombre_archivo>]+\n\n");
    exit(-1);
  }
  for(i=1;i<argc;i++) {
    printf("%s: ", argv[i]);
    if(lstat(argv[i],&atributos) < 0) {
      printf("\nError al intentar acceder a los atributos de %s",argv[i]);
      perror("\nError en lstat");
    }
    else {
      if(S_ISREG(atributos.st_mode))
        strcpy(tipoArchivo,"Regular");

      else if(S_ISDIR(atributos.st_mode))
        strcpy(tipoArchivo,"Directorio");

      else if(S_ISCHR(atributos.st_mode))
        strcpy(tipoArchivo,"Especial de caracteres");

      else if(S_ISBLK(atributos.st_mode))
        strcpy(tipoArchivo,"Especial de bloques");

      else if(S_ISFIFO(atributos.st_mode))
        strcpy(tipoArchivo,"Cauce con nombre (FIFO)");

      else if(S_ISLNK(atributos.st_mode))
        strcpy(tipoArchivo,"Enlace relativo(soft)");

      else if(S_ISSOCK(atributos.st_mode))
        strcpy(tipoArchivo,"Socket");
      else
        strcpy(tipoArchivo,"Tipo de archivo desconocido");

      printf("%s\n",tipoArchivo);
    }
  }
  return 0;
}
~~~
**Dice el nombre y el tipo que es el archivo pasado como argumento al ejecutar el programa, esto es, si es un archivo regular, directorio,...**

**Ejercicio 4.** Define una macro en lenguaje C que tenga la misma funcionalidad que la macro `S_ISREG(mode)` usando para ello los flags definidos en `<sys/stat.h>` para el campo `st_mode` de la struct `stat`, y comprueba que funciona en un programa simple. Consulta en un libro de C o en internet cómo se especifica una macro con argumento en C. `#define S_ISREG2(mode) ...`
*Nota: Puede ser interesante para depurar la ejecución de un programa en C que utilice llamadas al sistema usar la orden strace. Esta orden, en el caso más simple, ejecuta un programa hasta que finalice e intercepta y muestra las llamadas al sistema que realiza el proceso junto con sus argumentos y devuelve los valores devueltos en la salida de error estándar o en un archivo si se especifica la opción -o. Obtén más información con man.*
~~~c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> //Por usar stat
#include <stdio.h> //Para definir macros
#include <stdlib.h> //Para usar exit
#include <string.h> //Para poder hacer strcpy

#define S_ISREG2(atribute) ((atribute.st_mode & S_IFMT) == S_IFREG)? true : false
// Equivalentemente: #define S_ISREG2(mode) ( (mode & 0170000) == 0100000)

int main(int argc, char* argv[]){
  int i;
  struct stat atribute;
  char tipoArchivo[30];
  if(argc<2) {
    printf("\nSintaxis de ejecucion: tarea2 [<nombre_archivo>]+\n\n");
    exit(-1);
  }

  for(i=1;i<argc;i++) {
    printf("%s: ", argv[i]);
    if(lstat(argv[i],&atribute) < 0) {
      printf("\nError al intentar acceder a los atributos de %s",argv[i]);
      perror("\nError en lstat");
    }
    if(S_ISREG2(atribute)) // if(S_ISREG2(atribute.st_mode))
      strcpy(tipoArchivo,"Regular");
    else
      strcpy(tipoArchivo,"No es un archivo regular");
    printf("%s\n",tipoArchivo);
  }
  return 0;
}
~~~
