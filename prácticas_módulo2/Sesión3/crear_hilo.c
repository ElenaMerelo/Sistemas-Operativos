#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <linux/sched.h>
#include <malloc.h>
#define _GNU_SOURCE

int variable=3;
int thread(void *p) {
  int tid_;
  printf("\nSoy el hijo\n");
  sleep(5);
  variable++;
  tid_ = syscall(SYS_gettid);
  printf("\nPID y TID del hijo:%d %d\n",getpid(),tid_);
  printf("\nEn el hijo la variable vale:%d\n",variable);
}

int main() {
  void **stack;
  int i, tid;
  stack = (void **)malloc(15000);

  if (!stack)
    return -1;

  i = clone(thread, (char*) stack + 15000, CLONE_VM|CLONE_FILES|CLONE_FS|
  CLONE_THREAD|CLONE_SIGHAND, NULL);
  sleep(5);

  if (i == -1)  //clone devuelve -1 si se ha producido algún error
    perror("Error en clone");

  tid = syscall(SYS_gettid);
  printf("\nPID y TID del padre:%d %d\n ",getpid(),tid);
  printf("\nEn el padre la variable vale:%d\n",variable);
  return 0;
}
