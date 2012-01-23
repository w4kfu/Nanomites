#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <unistd.h>
#include <sys/wait.h>

int	son_work(void)
{
  printf("Hello ! I'm the son :]\n");
  __asm__("int3");
  return (0);
}

void	print_regsinfo(pid_t son)
{
  struct user_regs_struct regs;

  ptrace (PTRACE_GETREGS, son, 0, &regs);
  printf("Regs.eip = %08lx\n", regs.eip);

}

int	father_work(pid_t son)
{
  int	status;

  /* ptrace (PTRACE_SETOPTIONS, son, 0, PTRACE_O_TRACESYSGOOD); */
  while (1)
    {
      waitpid(son, &status, 0);
      if (WIFSIGNALED (status))
	{
	  printf("WTF\n");
	  ptrace (PTRACE_KILL, son, 0, 0);
	}
      if (WIFSTOPPED (status) && WSTOPSIG (status))
	{
	  printf("int3 ?\n");
	  print_regsinfo(son);
	  ptrace(PTRACE_CONT, son, 0, 0);
	}
      if (WIFEXITED (status))
	return (0);
    }
}

int	main(void)
{
  pid_t son;

  if ((son = fork()) == -1)
    {
      perror("[-] fork()");
      exit (EXIT_FAILURE);
    }
  if (son == 0)
    {
      if (ptrace(PT_TRACE_ME, 0, 0, 0) < 0)
	{
	  perror("[-] ptrace()");
	  exit(EXIT_FAILURE);
	}
      son_work();
    }
  else
    {
      father_work(son);
    }
  return (0);
}
