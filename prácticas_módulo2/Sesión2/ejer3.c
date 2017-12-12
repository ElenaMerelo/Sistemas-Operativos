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
Compilación y enlazado: gcc ejer3.c -o ejer3
Ejecución: ./ejer3 pathname(opcional)
Autora: Elena Merelo Molina
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
    if(dp->d_name[0] != '.' && dp->d_name[strlen(dp->d_name)-1] != '~'){
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
      //Si es directorio y no es un archivo oculto
      else if(S_ISDIR(atributes.st_mode)){ //Si es un directorio
        printf("\n%s es un directorio", dp->d_name);
        info_dir(dp->d_name, &files, &total_size);  //Nos metemos dentro de él, y realizamos lo mismo que en el directorio padre
      }
      //else
        //printf("\n%s no es un archivo regular ni un directorio", dp->d_name);

    }
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
