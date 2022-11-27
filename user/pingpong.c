//Write a program that uses UNIX system calls to ''ping-pong''
// a byte between two processes over a pair of pipes, one for each direction. 

// 1. The parent should send a byte to the child; 

// 2. the child should print "<pid>: received ping",  where <pid> is its process ID, 
// 3. write the byte on the pipe to the parent, 
// 4. and exit; 

// 5. the parent should read the byte from the child, 
// 6. print "<pid>: received pong", 
// 7. and exit. 

// Your solution should be in the file user/pingpong.c.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int ps[2];
  int buffer[1]; // Dont Worry about racing, because it's useless.
  pipe(ps);
  if(fork()==0){
    // still in parent
    int pid = getpid(); 
    write(ps[0]," ",1); // 1
    read(ps[0],buffer,1); // 5
    printf("%d: received pong\n", pid); // 6
    sleep(10);
    exit(0); // 7
  }else{
    int pid = getpid(); 
    read(ps[1],buffer,1); // 2
    printf("%d: received ping\n", pid); // 3
    write(ps[1],buffer,1); // 4
    sleep(10);
    exit(0);
  }
  exit(0);
}
