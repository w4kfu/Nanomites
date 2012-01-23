#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


void sig_trap(int sig)
{
  sig = sig;
  printf("SIG TRAP !\n");
}

int main(void)
{

  if (signal(SIGTRAP, sig_trap) == SIG_ERR)
    {
      perror("signal()");
      exit(EXIT_FAILURE);
    }
  __asm__
    (
     "int3"
     );
  return (0);
}
