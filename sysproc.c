#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  int status;

  if(argint(0, &status) < 0) // Obtenemos el estado
    return -1;

  exit(status << 8); // Lo pasamos por parámetro
  return 0;  // not reached
}

int
sys_wait(void)
{
  int *status;
  if(argptr(0, (void**) &status, sizeof(int)) < 0) // Obtenemos el estado
    return -1;

  return wait(status); // Lo pasamos por parámetro
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;

  if(n > 0) {
    if((addr + n) >= KERNBASE)
      return -1;
    myproc()->sz += n;
  } else if(n < 0) {
    if(growproc(n) < 0)
      return -1;
  }

  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

int
sys_date(void)
{
  struct rtcdate *d;

  if(argptr(0, (void**) &d, sizeof(struct rtcdate)) < 0)
    return -1;

  cmostime(d);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Obtenemos la prioridad de un proceso. Si se ha ejecutado
// correctamente devuelve la prioridad, de lo contrario
// devuelve -1
int
sys_getprio(void)
{
  int pid;
  int priority;

  if(argint(0, &pid) < 0 || pid < 0)
    return -1;

  if((priority = getprio(pid)) < 0)
    return -1;

  return priority;
}

// Asignamos una prioridad a un proceso en concreto.
// Si devuelve 0 se ha ejecutado correctamente, de lo
// contrario devuelve -1
int
sys_setprio(void)
{
  int pid;
  int priority;

  if(argint(0, &pid) < 0 || pid < 0)
    return -1;

  if(argint(1, &priority) < 0)
    return -1;

  // Casteo necesario, de lo contrario no podemos pasarle la prioridad
  if(setprio(pid, (enum proc_prio) priority) < 0)
    return -1;
  
  return 0;
}
