/* Ejercicio 5: Escribir un programa, similar a la orden cp, que utilice para su implementación la
llamada al sistema mmap() y una función de C que nos permite copiar memoria, como por
ejemplo memcpy(). Para conocer el tamaño del archivo origen podemos utilizar stat() y para
establecer el tamaño del archivo destino se puede usar ftruncate().*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char *argv[]){
  int fd_i, fd_o, num, i;
  struct stat sb;
  char *memoria1, *memoria2;

  if(argc != 3){
    printf("\nArgumentos incorrectos. Ha de introducir dos archivos.");
    exit(EXIT_FAILURE);
  }

  umask(0);
  // Si no existe argv[1], lo creamos
  fd_i = open(argv[1], O_RDONLY|O_CREAT, S_IRWXU);
  if (fd_i == -1) {
    perror("\nError en el primer open");
    exit(EXIT_FAILURE);
  }

  if(fstat(fd_i, &sb) == -1){
    printf("\nError al hacer fstat.");
    exit(EXIT_FAILURE);
  }

  int size= sb.st_size;
  // Creamos la proyección solo de lectura del archivo que queremos copiar.
  memoria1 = (char *)mmap(0, size, PROT_READ, MAP_SHARED, fd_i, 0);
  if (memoria1 == MAP_FAILED) {
      perror("Falló la primera proyección");
      exit(EXIT_FAILURE);
  }

  umask(0);
  fd_o= open(argv[2], O_RDWR|O_CREAT, S_IRWXU);
  if(fd_o == -1){
    perror("\nError en el segundo open.");
    exit(EXIT_FAILURE);
  }

  //Truncamos la memoria para el segundo archivo abierto para escritura
  printf ("Ajustando el tamano del archivo %s a %d \n",argv[2], size);
  ftruncate(fd_o, size);

  memoria2= (char *) mmap(0, size, PROT_WRITE | PROT_EXEC, MAP_SHARED, fd_o, 0);
  if (memoria2 == MAP_FAILED) {
      perror("Falló la segunda proyección");
      exit(EXIT_FAILURE);
  }

  //copiamos los archivos a traves de memoria
  printf ("Copiando el archivo %s en el archivo %s\n",argv[1],argv[2]);
  memcpy(memoria2,memoria1,size);

  // Cerramos las proyecciones
  if (munmap (memoria1, size) == -1) {
      printf("Error al cerrar la primera proyección \n");
      exit(-1);
  }

  if (munmap (memoria2, size) == -1) {
      printf("Error al cerrar la segunda proyección \n");
      exit(-1);
  }
}
