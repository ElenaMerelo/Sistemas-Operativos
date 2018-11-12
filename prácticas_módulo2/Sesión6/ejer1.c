/*Implementa un programa que admita t argumentos. El primer argumento será una
orden de Linux; el segundo, uno de los siguientes caracteres “<” o “>”, y el tercero el nombre de
un archivo (que puede existir o no). El programa ejecutará la orden que se especifica como
argumento primero e implementará la redirección especificada por el segundo argumento hacia
el archivo indicado en el tercer argumento. Por ejemplo, si deseamos redireccionar la entrada
estándar de sort desde un archivo temporal, ejecutaríamos:
$> ./mi_programa sort "<" temporal
Nota. El carácter redirección (<) aparece entre comillas dobles para que no los interprete el
shell sino que sea aceptado como un argumento del programa mi_programa.
Modo de ejecución: ./ejer1 orden_linux \"<\" ó ./ejer1 ls \">\" archivo
Autora: Elena Merelo Molina
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
  if(argc != 4){
    printf("\nNúmero de argumentos incorrecto");
    exit(EXIT_FAILURE);
  }

  /*if(strcmp(argv[2], "<") != 0 || strcmp(argv[2], ">" != 0)){
    printf("\nHa de introducir una orden linux, un < ó > y el nombre de un archivo.");
    exit(EXIT_FAILURE);
  }*/

  int fd;
  //Si se pone orden_linux > archivo, redireccionamos la salida del primero al segundo
  if(strcmp(argv[2], ">") == 0){
    // Creamos el archivo temporal solo de escritura, si ya existía lo truncamos a cero,
    // al que queremos redireccionar la salida estándar
    if(fd= open("temporal", O_CREAT| O_WRONLY | O_TRUNC)){
      printf("\nError en open");
      exit(EXIT_FAILURE);
    }

    //Cerramos la salida estándar asignada al proceso llamador, para que se escriba en el archivo y no ahí
    close(STDOUT_FILENO);

    // Duplicamos fd en el descriptor 1, STDOUT_FILENO
    if(fcntl(fd, F_DUPFD, STDOUT_FILENO) == -1) {
      perror ("\nFallo en fcntl");
      exit(EXIT_FAILURE);
    }

    execlp(argv[1], argv[1], NULL);
  }
  else{ // Redireccionamos la entrada estándar de la orden desde un archivo temporal
    if ((fd = open (argv[3], O_RDONLY)) < 0){ // Abrimos archivo. O_RDONLY => solo lectura
			perror("Error en el open\n");
			exit(EXIT_FAILURE);
		}

		close(STDIN_FILENO); // Cerramos la entrada estándar

		if ((fcntl(fd, F_DUPFD, STDIN_FILENO)) == -1){ // Ajustamos la entrada estándar con el fichero
			perror("Error en fcntl\n");
			exit(EXIT_FAILURE);
		}

		// Ahora la entrada estándar es el fichero
		if((execlp(argv[1],"", NULL)) < 0){ //Ejecutamos la orden
			perror("Error en execlp\n");
			exit(EXIT_FAILURE);
		}
  }
}
