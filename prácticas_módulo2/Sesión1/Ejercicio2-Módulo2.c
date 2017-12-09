/*Enunciado:  Implementa un programa que acepte como argumento el nombre de un archivo (pathname),
lo abra y lo lea en bloques de tamaño 80 Bytes, y cree un nuevo archivo de salida, salida.txt, en el que debe aparecer la
siguiente información:
Bloque 1
// Aquí van los primeros 80 Bytes del archivo pasado como argumento.
Bloque 2
// Aquí van los siguientes 80 Bytes del archivo pasado como argumento.
...
Bloque n
// Aquí van los siguientes 80 Bytes del archivo pasado como argumento.

Si no se pasa un argumento al programa se debe utilizar la entrada estándar como archivo de
entrada.
*Modificación adicional.* ¿Cómo tendrías que modificar el programa para que una vez
finalizada la escritura en el archivo de salida y antes de cerrarlo, pudiésemos indicar en su
primera línea el número de etiquetas ''Bloque i'' escritas de forma que tuviese la siguiente
apariencia?:
El número de bloques es <no_bloques>
Bloque 1
// Aquí van los primeros 80 Bytes del archivo pasado como argumento.
Bloque 2
// Aquí van los siguientes 80 Bytes del archivo pasado como argumento.
...*/

//File: Ejercicio2-Módulo2.c
//Authors: Elena Merelo Molina y Antonio Gámiz Delgado
//Compilation: gcc Ejercicio2-Módulo2.c -o Ejercicio2-Módulo2
//Execution: ./Ejercicio2-Módulo2 prueba.txt. It generates salida.txt

#include<sys/types.h>
#include<sys/stat.h>
+#include<fcntl.h>
#include<stdio.h>
#include<errno.h>

char buffer1[80];

int main(int argc, char *argv[]){
  int fd, fd_o, i=0;
  if(argc != 2)
    fd=0;
  else{
    if( (fd=open(argv[1],O_CREAT|O_RDONLY,S_IRUSR|S_IWUSR))<0) {
      printf("\nError %d en open",errno);
      perror("\nError en open");
      exit(-1);
    }
  }

  if( (fd_o=open("salida.txt",O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR))<0) {
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
