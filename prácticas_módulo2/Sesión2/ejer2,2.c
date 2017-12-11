/*Enunciado: Realiza un programa en C utilizando las llamadas
al sistema necesarias que acepte como entrada:
+ Un argumento que representa el 'pathname' de un directorio.
+ Otro argumento que es un número octal de 4 dígitos (similar al
que se puede utilizar para cambiar los permisos en la llamada al sistema
`chmod`). Para convertir este argumento tipo cadena a un tipo numérico
puedes utilizar la función strtol. Consulta el manual en línea para conocer
sus argumentos.

El programa tiene que usar el número octal indicado en el segundo argumento
 para cambiar los permisos de todos los archivos que se encuentren en
 el directorio indicado en el primer argumento.
Además, debe proporcionar en la salida estándar una línea para cada
archivo del directorio que esté formada por:
<nombre_de_archivo> : <permisos_antiguos> <permisos_nuevos>
Si no se pueden cambiar los permisos de un determinado archivo se
debe especificar la siguiente información en la línea de salida:
<nombre_de_archivo> : <errno> <permisos_antiguos>.*/
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
