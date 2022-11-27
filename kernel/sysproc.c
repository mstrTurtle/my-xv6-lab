#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
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
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
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


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  // 这是: a system call that reports which pages have been accessed.
  // 它接收的参数是: three arguments. 
  // base: it takes the starting virtual address of the first user page to check. 
  //  len: it takes the number of pages to check. 
  // mask: it takes a user address to a buffer to store the results into a bitmask 
  //     (a datastructure that uses one bit per page and where the first page corresponds to the least significant bit). 
  uint64 base;
  int len;
  uint64 mask;

  unsigned int tmpMask = 0;
  pte_t *pte;

  int maxLen = 8*sizeof(int); 

  struct proc *p = myproc();

  // 接收参数, 并做错误处理
  if (argaddr(0, &base) < 0 || argint(1, &len) < 0 || argaddr(2, &mask) < 0)
    return -1;
  // 若len溢出, 要修正.
  if (len > maxLen)
    len = maxLen;

  for (int i = 0; i < len; i++) {
    pagetable_t pagetable = p->pagetable; // 现在处于用户0级页表

    if (base >= MAXVA)
      panic("pgaccess: base index access error");

    // 根据VA, 解析出0级页表PTE
    // 这是根据官网的hint, 用了vm.c里的walk
    pte = walk(pagetable, base, 0);
    
    if (pte == 0)
      return -1;
    if ((*pte & PTE_V) == 0)
      return -1;
    if ((*pte & PTE_U) == 0)
      return -1;
    if (*pte & PTE_A) { // 若此页被ACCESSED了, 那就更新procmask.
      tmpMask |= (1L << i);
      *pte &= (~PTE_A); // 清除Accessed标记.
    }
    base += PGSIZE; // 去访问下一个页表
  }

  return copyout(p->pagetable, mask, (char *)&tmpMask, sizeof(unsigned int));
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
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
