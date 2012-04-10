#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define CF_MASK 1 << 1 // bit 1
# define PF_MASK 1 << 2 // bit 2
# define AF_MASK 1 << 4 // bit 4
# define ZF_MASK 1 << 6 // bit 6
# define SF_MASK 1 << 7 // bit 7
# define OF_MASK 1 << 11 // bit 11

# define CF_SET(eflags) ((eflags & CF_MASK) & 1)
# define PF_SET(eflags) ((eflags & PF_MASK) & 1)
# define AF_SET(eflags) ((eflags & AF_MASK) & 1)
# define ZF_SET(eflags) ((eflags >> 6) & 1)
# define SF_SET(eflags) ((eflags & SF_MASK) & 1)
# define OF_SET(eflags) ((eflags & OF_MASK) & 1)

extern void son_work(void);

long activate_debug(void)
{
  if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0)
    return (-1);
  return (0);
}

long regs_read(pid_t pid, struct user_regs_struct *regs)
{
  memset(regs, 0, sizeof (struct user_regs_struct));
  if (ptrace(PTRACE_GETREGS, pid, NULL, regs) == -1)
    return (-1);
  return (0);
}

long cont_signal(pid_t pid, int signal)
{
  if (ptrace(PTRACE_CONT, pid, NULL, signal) == -1)
    return (-1);
  return (0);
}
long regs_write(pid_t pid, struct user_regs_struct *regs)
{
  if (ptrace(PTRACE_SETREGS, pid, NULL, regs) == -1)
    return (-1);
  return (0);
}

void callback_sigtrap(pid_t pid)
{
  static int first = 1;
  unsigned addr;
  struct user_regs_struct regs;
  int status;
  char type_j;
  unsigned long dest;

  regs_read(pid, &regs);
  printf("EIP = %lX\n", regs.eip);
  type_j = *((char*)regs.eip);
  dest = *((long*)(regs.eip + 1));
  printf("dest = %08X\n", dest);
  switch (type_j)
    {
    case 1:
      printf("EFLAGS = %x\n", regs.eflags);
      printf("JZ and JE\n");
      if ((regs.eflags >> 6) & 1)
	regs.eip += dest + 1;
      else
	regs.eip += 5;
      break;
    case 2:
      printf("JNZ and JNE\n");
      break;
    case 3:
      printf("JB and JC\n");
      break;
    case 4:
      printf("JNB and JNC\n");
      break;
    case 5:
      printf("JMP !\n");
      regs.eip += dest + 1;
      break;
    default:
      printf("WTF !\n");
      break;
    }

  printf("New eip = %08X\n", regs.eip);
  regs_write(pid, &regs);
  cont_signal(pid, 0);
}

long mem_read(pid_t pid, unsigned addr)
{
  ptrace(PTRACE_PEEKDATA, pid, addr, 0);
}


void mem_print_chars(pid_t pid, unsigned addr)
{
  unsigned data;

  data = mem_read(pid, addr);
  fprintf(stderr, "@%8x : %02x %02x %02x %02x\n", addr,
          data % 0x100, (data >> 8) % 0x100,
          (data >> 16) % 0x100, (data >> 24) % 0x100);
}

void regs_dump(pid_t pid)
{
  struct user_regs_struct regs;
  if (regs_read(pid, &regs) == -1)
    fprintf(stderr, "Erreur regs_read !\n");
  fprintf(stderr, "eax=%08x ebx=%08x ecx=%08x edx=%08x\n",
          regs.eax, regs.ebx, regs.ecx, regs.edx);
  fprintf(stderr, "esi=%08x edi=%08x\n", regs.esi, regs.edi);
  fprintf(stderr, "eip=%08x esp=%08x ebp=%08x\n",
          regs.eip, regs.esp, regs.ebp);
}


void callback_generic(pid_t pid, int signal)
{
  unsigned addr;
  struct user_regs_struct regs;

  regs_read(pid, &regs);
  addr = regs.eip;
  if (signal == SIGILL)
    printf("Illegal instruction @%x\n", addr);
  else if (signal == SIGSEGV)
    printf("Segmentation fault @%x\n", addr);
  else
    printf("signal %d @%x\n", signal, addr);
  mem_print_chars(pid, addr);
  regs_dump(pid);
  exit(1);
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
          signal = WSTOPSIG(status);
          if (signal == SIGTRAP)
	    callback_sigtrap(pid);
          else
	    callback_generic(pid, signal);
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
       	exit(2);
      else
	son_work();
    }
  return (0);
}
