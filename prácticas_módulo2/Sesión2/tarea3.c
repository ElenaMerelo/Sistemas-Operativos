/*
tarea3.c
Trabajo con llamadas al sistema del Sistema de Archivos ''POSIX 2.10 compliant'' Este programa fuente está pensado para que se cree primero un programa con la parte de CREACION DE ARCHIVOS y se haga un ls -l para fijarnos en los permisos y entender la llamada umask.
En segundo lugar (una vez creados los archivos) hay que crear un segundo programa con la parte de CAMBIO DE PERMISOS para comprender el cambio de permisos relativo a los permisos que actualmente tiene un archivo frente a un establecimiento de permisos absoluto.
*/
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>

int main(int argc, char *argv[]){
  int fd1,fd2;
  struct stat atributos;

  if(argc != 3) {
    printf("\nHa de pasar dos archivos como argumento");
    exit(-1);
  }

  //CREACION DE ARCHIVOS
  /*Crea un archivo en el que solo se puede escribir, para el cual el grupo
  tiene permiso de escritura, lectura y ejecución. Con el flag O_TRUNC si el archivo
  ya existía, es regular y su modo de acceso permite escritura, será truncado a longitud 0.
  Si el archivo es un FIFO o un terminal device file, el flag será ignorado.
  */
  if((fd1=open(argv[1],O_CREAT|O_TRUNC|O_WRONLY,S_IRGRP|S_IWGRP|S_IXGRP))<0){
    printf("\nError %d en open(archivo1,...)",errno);
    perror("\nError en open");
    exit(-1);
  }

  /*Se pone umask a 0, lo que permite que cualesquiera bits sean
  modificados al usar open(), no se anula ningún permiso, pero tampoco se establece
  ninguno, por eso luego hay que hacerlo con open()*/
  umask(0);
  if((fd2=open(argv[2],O_CREAT|O_TRUNC|O_WRONLY,S_IRGRP|S_IWGRP|S_IXGRP))<0){
    printf("\nError %d en open(archivo2,...)",errno);
    perror("\nError en open");
    exit(-1);
  }

  //CAMBIO DE PERMISOS
  if(stat(argv[1],&atributos) < 0){
    printf("\nError al intentar acceder a los atributos de archivo1");
    perror("\nError en lstat");
    exit(-1);
  }

  //Quita el permiso de ejecución al grupo y establece el bit de grupo
  if(chmod(argv[1], (atributos.st_mode & ~S_IXGRP) | S_ISGID) < 0){
    perror("\nError en chmod para archivo1");
    exit(-1);
  }

  /*Da permisos de lectura, escritura y ejecución al usuario, de lectura
   y escritura al grupo y de lectura a otros.*/
  if(chmod(argv[2],S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH) < 0){
    perror("\nError en chmod para archivo2");
    exit(-1);
  }
  close(fd1);
  close(fd2);
  return 0;
}
