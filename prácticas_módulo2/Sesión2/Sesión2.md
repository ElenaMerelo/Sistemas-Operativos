## Sesión 2. Llamadas al sistema para el SA (Parte II)
### Actividad 2.1 Trabajo con llamadas de cambio de permisos
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
**Este programa crea dos archivos, "archivo1" (en el que solo se puede escribir, para el cual el grupo tiene permiso de ejecución, lectura y escritura, especificando O_TRUNC por si el archivo ya existía, para vaciarlo) y "archivo2" (puesto todo a 0 con umask(0), también truncado, solo de escritura, con permisos de lectura, escritura y ejecucion para el grupo), y luego cambiamos los permisos, de manera que el grupo ya no puede ejecutar "archivo1" y estableciendo el bit de grupo, y dándole permisos de lectura, escritura y ejecución al usuario, de lectura y escritura al grupo y de lectura a otros en "archivo2". Como consta en: https://github.com/DGIIMUnderground/DGIIM2/blob/master/C1/SO/Practicas/ModuloII/Sesion2.md:

En archivo1, de primeras se añade lectura escritura y ejecución para el grupo, que al pasarle la máscara ~022, se hace la operación 000 111 000 & 111 101 101 = 000 101 000(Se puede decir que se le quitan los permisos de la máscara). Antes de la llamada a chmod esos son los permisos que tiene archivo1. chmod toma los permisos actuales y les quita el permiso de ejecución al grupo (atributos.st_mode & ~S_IXGRP), para después activar la asignación del GID propietario al GID efectivo. Esta acción, al hacer ls -l se ve codificada con una S en el bit correspondiente a la ejecución del grupo. Por este motivo, tras ejecutar el programa, en archivo1 el comando ls -l nos devuelve la siguiente secuencia: ---r-S---.
Para archivo2, de primeras open le había asignado unos permisos que, como la máscara estaba establecida a 000 se mantuvieron iguales. Sin embargo al hacer chmod no se tienen en cuenta los permisos que tenía anteriormente, simplemente se le asignan unos nuevos. Se activa la lectura, escritura y ejecución para el usuario y permite lectura y escritura al grupo y lectura para otros. Si no se ha podido cambiar alguno, se sale del programa. Si ha ido bien, termina la ejecución.

 Así, al compilar el programa, ejecutarlo y ver los permisos que tienen los archivos nuevos obtenemos:**
~~~shell
> gcc tarea3.c -o tarea3
> ./tarea3
> ls -l archivo*
-rwxrwSrwx 1 elena elena  0 nov  9 12:14 archivo1
-rwxrw-r-- 1 elena elena  0 nov  9 12:14 archivo2
~~~

### Actividad 2.2 Trabajo con funciones estándar de manejo de directorios
**Ejercicio 2.** Realiza un programa en C utilizando las llamadas al sistema necesarias que acepte como entrada:
+ Un argumento que representa el 'pathname' de un directorio.
+ Otro argumento que es un número octal de 4 dígitos (similar al que se puede utilizar para cambiar los permisos en la llamada al sistema `chmod`). Para convertir este argumento tipo cadena a un tipo numérico puedes utilizar la función strtol. Consulta el manual en línea para conocer sus argumentos.

El programa tiene que usar el número octal indicado en el segundo argumento para cambiar los permisos de todos los archivos que se encuentren en el directorio indicado en el primer argumento.
Además, debe proporcionar en la salida estándar una línea para cada archivo del directorio que esté formada por:
*nombre_de_archivo : permisos_antiguos permisos_nuevos*
Si no se pueden cambiar los permisos de un determinado archivo se debe especificar la siguiente información en la línea de salida:
*nombre_de_archivo : errno permisos_antiguos*
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

  DIR * dir;
  struct dirent * dp;

  dir= opendir(argv[1]);
  if (dir == NULL) {
    printf("\nError %d en opendir(%s)", errno, argv[1]);
    perror ("Error en opendir");
    exit (-1);
  }

  char * endptr;
  int mask = strtol(argv[2], NULL, 8);
  mode_t old_mask;

  struct stat atributes;
  char complete_name [500];

  //Leemos las entradas del directorios
  while((dp= readdir(dir)) != NULL){
    if (strcmp (dp->d_name, "..") != 0 && strcmp (dp->d_name, ".") != 0){
      strcpy(complete_name, argv[1]);
      strcat(complete_name, "/");
      strcat(complete_name,elemento_dir->d_name);

      if(stat(complete_name, &atributes) < 0){
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
~~~c
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
void info_dir(const char *pathname, int * reg_files, long long int * total_size, long long int * reg_size){
  DIR * dir= NULL;
  struct dirent * dp= NULL;
  struct stat atributes;
  char name[500];

  dir= opendir(pathname);
  //Abrimos el flujo del directorio pasado como argumento
  if(dir == NULL){
    printf("\nError %d al abrir el directorio %s", errno, pathname);
    perror("Error en opendir");
    exit(-1);
  }

  /* Recorremos ahora lo que hay en el directorio
  Con S_IXGRP  comprobamos si el grupo tiene permisos de ejecución
  Con  S_IXOTH comprobamos si otros tienen permiso de ejecución
  Con S_ISREG(mode) vemos si el archivo es regular, si lo es incrementamos el contado de archivos regulares
  * /
  while((dp= readdir(dir)) != NULL){
    //Si no es un archivo oculto
    if(dp->d_name[0] != '.' && strcmp(dp->d_name, "..") != 0 && dp->d_name[strlen(dp->d_name)-1] != '~'){
      strcpy(name, pathname);
      strcat(name, "/");
      strcat(name,dp->d_name);

      if(stat(name, &atributes) < 0){
        printf("\nError al intentar acceder a los atributos de %s\n", dp->d_name);
        perror("\nError en stat\n");
        // exit(-1);
      }

      (* total_size) += atributes.st_size;

      if(S_ISREG(atributes.st_mode)){ //Si el archivo es regular
        printf("\n%s es un archivo regular", dp->d_name);

        if(atributes.st_mode & (S_IXGRP | S_IXOTH)){  //Si group y others tienen permiso de ejecución
          printf("\n%s es un archivo regular y tiene permisos de ejecución para others y group", dp->d_name);
          (* reg_files)++;
          (* reg_size) += (int) atributes.st_size;
          printf("\n Nombre: %s, i-nodo: %ld", dp->d_name, dp->d_ino);
        }
        else
          printf("\n%s no tiene permisos de ejecución para others y group", dp->d_name);
      }

      //Si es directorio y no es un archivo oculto
      else if(S_ISDIR(atributes.st_mode)){ //Si es un directorio
        printf("\n%s es un directorio", dp->d_name);
        info_dir(dp->d_name, &reg_files, &total_size, &reg_size);  //Nos metemos dentro de él, y realizamos lo mismo que en el directorio padre
      }
    }
    else
      printf("\n%s no es un archivo regular ni un directorio", dp->d_name);
  }
  closedir(dir);
}

int main(int argc, char *argv[]){
  int reg_files= 0;
  long long int total_size= 0, reg_size= 0;

  if(argc > 2){
    printf("\nModo de ejecución: ./ejer3 (./directorio)\n");
    exit(-1);
  }

  if(argc == 2)
    info_dir(argv[1], &reg_files, &total_size, &reg_size);
  else
    info_dir(".", &reg_files, &total_size, &reg_size);

  printf("\nExisten %d archivos regulares con permisos de ejecución para group y others\n", reg_files);
  printf("\nEl tamaño total ocupado por archivos regulares es %lld bytes y por el resto de archivos %lld\n", reg_size, total_size);
}

~~~
### Actividad 2.3 Trabajo con la llamada `nftw()` para recorrer un sistema de archivos
**Ejercicio 4.** Implementa de nuevo el programa buscar del ejercicio 3 utilizando la llamada al sistema `nftw`.
~~~c
/*
Archivo: ejercicio4.c
Compilación y enlazado: gcc ejercicio4.c -o ejercicio4
Ejecución: ./ejercicio 4 [pathname]
Autora: Elena Merelo Molina
*/
#define _XOPEN_SOURCE 500 //Si no da error al usar los flags de nftw()
#include<ftw.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>

/*Declaramos las funciones size y total como globales, al no poder pasárselas a las funciones info_dir y nftw como argumento, el
prototipo de ambas ha de ser: int nftw(const char *dirpath,
               int (*fn) (const char *fpath, const struct stat *sb,
                          int typeflag, struct FTW *ftwbuf),
               int nopenfd, int flags);
*/
int n_files = 0;
long int size = 0;

int info_dir(const char* path, const struct stat* sb, int typeflag, struct FTW* ftwbuf){
	//Comprobamos que los archivos sean regulares y sean ejecutables para grupo y otros
	if (typeflag == FTW_F && (sb->st_mode & (S_IXGRP|S_IXOTH))){
    //Los inodos son unsigned long long, así que para que los pinte sprintf hay que usar llu
    printf("%s %llu\n",path, sb->st_ino);

    //Incrementamos las variables globales
    size += sb  ->st_size;
    n_files++;

	}
	return 0;
}

int main(int argc, char *argv[]){
  if(argc > 2){
    printf("\nModo de ejecución: ./ejer3 ./directorio\n");
    exit(-1);
  }

  printf("\nLos i-nodos son:");
  //Si no se pasan argumentos se usa como directorio el actual(también se podría haber puesto get_current_dir_name()), sino el pasado como argumento
  if(nftw((argc >= 2) ? argv[1] : ".", info_dir, 10, 0) == 0){
    perror("nftw");
  }

  printf("\nExisten %d archivos regulares con permisos de ejecución para group y others\n", n_files);
  printf("\nEl tamaño total ocupado por dichos archivos es %d bytes\n", size);

}
~~~


### Extra:

~~~
#### Ejemplo de uso de getcwd
~~~c

/* CELEBG03
   This example determines the working directory.
 */
#define _POSIX_SOURCE
#include <unistd.h>
#undef _POSIX_SOURCE
#include <stdio.h>

main() {
  char cwd[256];

  if (chdir("/tmp") != 0)
    perror("chdir() error()");
  else {
    if (getcwd(cwd, sizeof(cwd)) == NULL)
      perror("getcwd() error");
    else
      printf("current working directory is: %s\n", cwd);
  }
}
~~~
