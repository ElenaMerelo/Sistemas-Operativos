/*Enunciado:Implementa de nuevo el programa buscar del ejercicio 3 utilizando la llamada al
sistema nftw.
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
