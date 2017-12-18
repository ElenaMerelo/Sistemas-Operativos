//tarea11.c
/*Programa que suspende la ejecución del proceso actual hasta que reciba una
señal distinta de SIGUSR1.*/
#include <stdio.h>
#include <signal.h>
int main(){
  sigset_t new_mask;
  /*inicializar la nueva mascara de señales*/
  sigemptyset(&new_mask);
  sigaddset(&new_mask, SIGUSR1);
  /*esperar a cualquier señal excepto SIGUSR1*/
  sigsuspend(&new_mask);
}
