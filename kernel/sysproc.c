#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


uint64
sys_trace(void){
  // trace 32 grep hello README
  int mask;
  argint(0, &mask);

  myproc()->trace_bmp  = mask;
  return 0;
}

uint64
sys_sysinfo(void)
{
  struct sysinfo info;
  info.freemem = freemem();
  info.nproc = procnum();

  uint64 addr;
  argaddr(0, &addr);

  if(copyout(myproc()->pagetable, addr, (char *)&info, sizeof(info)) < 0)
    return -1;
  return 0;

}

/*
1. pagetable_t 可以从`proc`结构体获取
2. dstva  目的虚拟地址----通过argaddr() 获取到一个用户地址，本质上是通过proc->trapframe获取。然后这个va能不能用，不知道，只确保地址是合法的。能不能用交给copyout去遍历。
3. src是源数据地址（应该也是虚拟地址）--- 就是sysinfo的地址
4. len可以根据实际的数据大小填写
*/