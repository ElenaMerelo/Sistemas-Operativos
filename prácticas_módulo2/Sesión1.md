#Sesión 1. Llamadas al sistema para el Sistema de archivos (Parte I)
**Ejercicio 1.** ¿Qué hace el siguiente programa? Probad tras la ejecución del programa las siguientes órdenes del shell: `cat archivo` y `od -c archivo`
~~~
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

  /*Crea un archivo en modo solo escritura, y S_IRUSR indica que el usuario 00400
  tiene permiso de lectura, S_IWUSR que el usuario 00200 tiene permisos de escritura.
  Si la llamada al sistema open devuelve un valor negativo es que ha fallado. */
  if( (fd=open("archivo",O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR))<0) {  
    printf("\nError %d en open",errno);
    perror("\nError en open");
    exit(-1);
  }

  /*Notificamos cuando se escriban en el archivo un número distinto a 10 bytes, que es
  el tamaño del bf1. Esto puede ocurrir por ejemplo si no hay suficiente memoria.
  Esta llamada escribe los 10 bytes del buffer apuntado por buf1 en el fichero referido
  if(write(fd,buf1,10) != 10) {
    perror("\nError en primer write");
    exit(-1);
  }

  /* Se pone el offset a 40 bytes. Si funciona correctamente devuelve la localización
  del offset en bytes contada desde el principio del archivo. Si se ha producido un error
  se devuelve -1./
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
~~~
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
~~~
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
    if((fd=open(argv[1],O_CREAT|O_RDONLY,S_IRUSR|S_IWUSR))<0) {
      printf("\nError %d en open",errno);
      perror("\nError en open");
      exit(-1);
    }
  }

  if((fd_o=open("salida.txt",O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR))<0) {
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








#
