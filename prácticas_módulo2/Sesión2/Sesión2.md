##Sesión 2. Llamadas al sistema para el SA (Parte II)
###Actividad 2.1 Trabajo con llamadas de cambio de permisos
**Ejercicio 1.** ¿Qué hace el siguiente programa?
~~~c
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
  //CREACION DE ARCHIVOS
  if((fd1=open("archivo1",O_CREAT|O_TRUNC|O_WRONLY,S_IRGRP|S_IWGRP|S_IXGRP))<0){
    printf("\nError %d en open(archivo1,...)",errno);
    perror("\nError en open");
    exit(-1);
  }

  umask(0);
  if((fd2=open("archivo2",O_CREAT|O_TRUNC|O_WRONLY,S_IRGRP|S_IWGRP|S_IXGRP))<0){
    printf("\nError %d en open(archivo2,...)",errno);
    perror("\nError en open");
    exit(-1);
  }

  //CAMBIO DE PERMISOS
  if(stat("archivo1",&atributos) < 0){
    printf("\nError al intentar acceder a los atributos de archivo1");
    perror("\nError en lstat");
    exit(-1);
  }

  if(chmod("archivo1", (atributos.st_mode & ~S_IXGRP) | S_ISGID) < 0){
    perror("\nError en chmod para archivo1");
    exit(-1);
  }
  if(chmod("archivo2",S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH) < 0){
    perror("\nError en chmod para archivo2");
    exit(-1);
  }
  close(fd1);
  close(fd2);
  return 0;
}
~~~
**El programa cambia los permisos de los archivos pasados como argumento. Veamos cómo: Creo dos archivos, test.txt y test2.txt, cuyos permisos iniciales son `-rw-rw-r--`. Al ejecutar el programa pasándolos como argumento, escribiendo desde la terminal `gcc tarea3.c -o tarea3`, y luego `./tarea3 test.txt test2.txt` devuelve el siguiente error: `Error en open: Permission denied
Error 13 en open(archivo1,...)`, incluso habiendo hecho antes `chmod a+x test.txt`. Modificamos pues el archivo original para que se pasen obligatoriamente dos archivos, y las operaciones realizadas en el programa se hagan sobre éstos, incluyendo al principio del código:**
~~~c
if(argc<2) {
  printf("\nHa de pasar dos archivos como argumento");
  exit(-1);
}
~~~
**y cambiando donde antes ponía archivoi por argv[i]. Ahora recompilamos y ejecutamos, y vemos cómo efectivamente ha cambiado de la forma esperada los permisos de test.txt y otro archivo creado de la misma forma test2.txt:**
~~~
$ls -l test*.txt
-rwxrw-r-- 1 elena elena 0 dic  9 16:18 test2.txt
-rwxrwSr-x 1 elena elena 0 dic  9 16:18 test.txt
~~~
###Actividad 2.2 Trabajo con funciones estándar de manejo de directorios
**Ejercicio 2.** Realiza un programa en C utilizando las llamadas al sistema necesarias que acepte como entrada:
+ Un argumento que representa el 'pathname' de un directorio.
+ Otro argumento que es un número octal de 4 dígitos (similar al que se puede utilizar para cambiar los permisos en la llamada al sistema `chmod`). Para convertir este argumento tipo cadena a un tipo numérico puedes utilizar la función strtol. Consulta el manual en línea para conocer sus argumentos.

El programa tiene que usar el número octal indicado en el segundo argumento para cambiar los permisos de todos los archivos que se encuentren en el directorio indicado en el primer argumento.
Además, debe proporcionar en la salida estándar una línea para cada archivo del directorio que esté formada por:
<nombre_de_archivo> : <permisos_antiguos> <permisos_nuevos>
Si no se pueden cambiar los permisos de un determinado archivo se debe especificar la siguiente información en la línea de salida:
<nombre_de_archivo> : <errno> <permisos_antiguos>
~~~c
// Nombre del archivo: ejer2,2.c
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>
#include <dirent.h>

int main(int argc, char *argv[]){
  if(argc != 3){
    printf("\nModo de ejecución: ./ejer2,2 ./pathname número\n");
    exit(-1);
  }

  DIR *dir;
  struct dirent *dp;

  dir= opendir(argv[1]);
  if (dir == NULL) {
    printf("\nError %d en opendir(%s)", errno, argv[1]);
    perror ("Error en opendir");
    exit (-1);
  }

  char *endptr;
  mode_t mask= strtoul(argv[2], &endptr, 8);
  mode_t old_mask;

  struct stat atributes;

  //Leemos las entradas del directorios
  while((dp= readdir(dir)) != NULL){
    if(stat(dp->d_name, &atributes) < 0){
      printf("\nError al intentar acceder a los atributos de %s\n", dp->d_name);
      perror("\nError en stat\n");
      exit(-1);
    }
    old_mask= atributes.st_mode;
    printf("%s\n", dp->d_name);
  	if((chmod(dp->d_name, mask)) < 0 )
  		printf("\n%s: %d\t %d\t %s", dp->d_name, errno, old_mask, "error");
  	else
  		printf("\n%s: %d\t %d", dp->d_name, old_mask, atributes.st_mode);
  }
  closedir(dir);
}
~~~
**Ejercicio 3.** Programa una nueva orden que recorra la jerarquía de subdirectorios existentes a partir de uno dado como argumento y devuelva la cuenta de todos aquellos archivos regulares que tengan permiso de ejecución para el grupo y para otros. Además del nombre de los archivos encontrados, deberá devolver sus números de inodo y la suma total de espacio ocupado por dichos archivos. El formato de la nueva orden será:
`$> ./buscar <pathname>`
donde <pathname> especifica la ruta del directorio a partir del cual queremos que empiece a analizar la estructura del árbol de subdirectorios. En caso de que no se le de argumento, tomará como punto de partida el directorio actual. Ejemplo de la salida después de ejecutar el programa:
Los i-nodos son:
./a.out 55
./bin/ej 123
./bin/ej2 87
...
Existen 24 archivos regulares con permiso x para grupo y otros
El tamaño total ocupado por dichos archivos es 2345674 bytes
```c
/*Ejercicio 3. Programa una nueva orden que recorra la jerarquía de subdirectorios
existentes a partir de uno dado como argumento y devuelva la cuenta de todos aquellos
archivos regulares que tengan permiso de ejecución para el grupo y para otros.
Además del nombre de los archivos encontrados, deberá devolver sus números de inodo
y la suma total de espacio ocupado por dichos archivos. El formato de la nueva orden será:
$> ./buscar <pathname>
donde <pathname> especifica la ruta del directorio a partir del cual queremos que empiece a
analizar la estructura del árbol de subdirectorios. En caso de que no se le de argumento,
tomará como punto de partida el directorio actual. Ejemplo de la salida después de
ejecutar el programa:
Los i-nodos son:
./a.out 55
./bin/ej 123
./bin/ej2 87
...
Existen 24 archivos regulares con permiso x para grupo y otros
El tamaño total ocupado por dichos archivos es 2345674 bytes
*/

#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>
#include <dirent.h>
/*Función recursiva que, dado un directorio, cuenta los archivos regulares con
permiso de ejecución para others y group que hay en él y su tamaño. Si encuentra
un directorio dentro hace lo mismo, y así sucesivamente hasta que no haya más directorios
ni archivos. Hacemos los punteros constantes para que no se pueda modificar el lugar al
que apuntan pero sí a lo que apuntan.
*/
void info_dir(const char *pathname, int * const files, long long int * const total_size){
  DIR *dir= NULL;
  struct dirent *dp= NULL;
  struct stat atributes;

  dir= opendir(pathname);
  //Abrimos el flujo del directorio pasado como argumento
  if(dir == NULL){
    printf("\nError %d al abrir el directorio %s", errno, pathname);
    perror("Error en opendir");
    exit(-1);
  }

  /*Recorremos ahora lo que hay en el directorio
  Con S_IXGRP  comprobamos si el grupo tiene permisos de ejecución
  Con  S_IXOTH comprobamos si otros tienen permiso de ejecución
  Con S_ISREG(mode) vemos si el archivo es regular, si lo es incrementamos el contado de archivos regulares
  */
  while((dp= readdir(dir)) != NULL){
    if(stat(dp->d_name, &atributes) < 0){
      printf("\nError al intentar acceder a los atributos de %s\n", dp->d_name);
      perror("\nError en stat\n");
      exit(-1);
    }

    if(S_ISREG(atributes.st_mode)){ //Si el archivo es regular
      //printf("\n%s es un archivo regular", dp->d_name);
      if(atributes.st_mode & (S_IXGRP | S_IXOTH)){  //Si group y others tienen permiso de ejecución
        printf("\n%s es un archivo regular y tiene permisos de ejecución para others y group", dp->d_name);
        (*files)++;
        (*total_size) += atributes.st_size;
        printf("\n Nombre: %s, i-nodo: %d", dp->d_name, dp->d_ino);
        printf("\nNúmero de archivos regulares: %d, total de bytes: %lld", &files, &total_size);
      }
      //else
        //printf("\n%s no tiene permisos de ejecución para others y group", dp->d_name);
    }

    else if(S_ISDIR(atributes.st_mode)){ //Si es un directorio
      printf("\n%s es un directorio", dp->d_name);
      info_dir(dp->d_name, &files, &total_size);  //Nos metemos dentro de él, y realizamos lo mismo que en el directorio padre
    }
    //else
      //printf("\n%s no es un archivo regular ni un directorio", dp->d_name);

  }
  closedir(dir);

}

int main(int argc, char *argv[]){
  int files= 0;
  long long int total_size= 0;
  if(argc > 2){
    printf("\nModo de ejecución: ./ejer3 ./directorio\n");
    exit(-1);
  }

  if(argc == 2)
    info_dir(argv[1], &files, &total_size);
  else
    info_dir(get_current_dir_name(), &files, &total_size);

  printf("\nExisten %d archivos regulares con permisos de ejecución para group y others\n", files);
  printf("\nEl tamaño total ocupado por dichos archivos es %d bytes\n", total_size);
}
```



### Extra:
~~~
NAME
       getumask - get file creation mask

SYNOPSIS
       #define _GNU_SOURCE             /* See feature_test_macros(7) */
       #include <sys/types.h>
       #include <sys/stat.h>

       mode_t getumask(void);

DESCRIPTION
       This function returns the current file creation mask.  It is equivalent
       to

           mode_t getumask(void)
           {
               mode_t mask = umask( 0 );
               umask(mask);
               return mask;
           }

       except that it is documented to be thread-safe (that is, shares a  lock
       with the umask(2) library call).
       NAME
              chown, fchown, lchown, fchownat - change ownership of a file

       SYNOPSIS
              #include <unistd.h>

              int chown(const char *pathname, uid_t owner, gid_t group);
              int fchown(int fd, uid_t owner, gid_t group);
              int lchown(const char *pathname, uid_t owner, gid_t group);

              #include <fcntl.h>           /* Definition of AT_* constants */
              #include <unistd.h>

              int fchownat(int dirfd, const char *pathname,
                           uid_t owner, gid_t group, int flags);

          Feature    Test   Macro   Requirements   for   glibc   (see   fea‐
          ture_test_macros(7)):

              fchown(), lchown():
                  _BSD_SOURCE || _XOPEN_SOURCE >= 500 ||
                  _XOPEN_SOURCE && _XOPEN_SOURCE_EXTENDED
                  || /* Since glibc 2.12: */ _POSIX_C_SOURCE >= 200809L

              fchownat():
                  Since glibc 2.10:
                      _XOPEN_SOURCE >= 700 || _POSIX_C_SOURCE >= 200809L
                  Before glibc 2.10:
                      _ATFILE_SOURCE

       DESCRIPTION
              These  system calls change the owner and group of a file.  The
              chown(), fchown(), and lchown() system calls  differ  only  in
              how the file is specified:

              * chown() changes the ownership of the file specified by path‐
                name, which is dereferenced if it is a symbolic link.

              * fchown() changes the ownership of the file  referred  to  by
                the open file descriptor fd.
              * lchown()  is like chown(), but does not dereference symbolic links.
~~~

#
