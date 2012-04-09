#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long activate_debug(void)
{
  if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0)
    return (-1);
  return (0);
}

long regs_read(pid_t pid, struct user_regs_struct *regs)
{
  memset(regs, 0, sizeof (struct user_regs_struct));
  ptrace(PTRACE_GETREGS, pid, NULL, regs);
}

long cont_signal(pid_t pid, int signal)
{
  ptrace(PTRACE_CONT, pid, NULL, signal);
}

void callback_sigtrap(pid_t pid)
{
  static int first = 1;
  unsigned addr;
  struct user_regs_struct regs;
  int status;

  regs_read(pid, &regs);
  printf("EIP = %08X\n", regs.eip);
  printf("Opcode = %02X\n", *((char*)regs.eip - 1));
  printf("LOL WAT\n");
  cont_signal(pid, 0);
}

void debug_loop(pid_t pid)
{
  int status;
  int signal;

  while (1)
    {
      waitpid(pid, &status, 0);
      if (WIFSTOPPED(status))
        {
          /* signal ptrace */
          signal = WSTOPSIG(status);
          if (signal == SIGTRAP)
	    callback_sigtrap(pid);
          /* else */
          /*   callback_generic(pid, signal); */
        }
      else
        {
          fprintf(stderr, "Signal, exit !\n");
          break;
        }
    }
}

int main(void)
{
  pid_t pid;

  pid = fork();
  if (pid == -1)
    exit(42);
  if (pid != 0)
    {
      debug_loop(pid);
      exit(0);
    }
  else
    {
      if (activate_debug() == -1)
	{
	  exit(2);
	}
      else
	{
	  __asm__("int3");
	}
    }

  return (0);
}
