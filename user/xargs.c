#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
// Write a simple version of the UNIX find program: 
// find all the files in a directory tree with a specific name. 
// Your solution should be in the file user/find.c.
int argBeg ;
int argEnd ;
char* newArgv[100];

void fire(char* newLine){
  newArgv[argEnd-argBeg] = newLine;
  newArgv[argEnd-argBeg+1] = 0;
  int retPid;
  if((retPid=fork())){
    wait(0);
    return ;
  }// fork return child pid in parent
  else{
    printf("In Child.\n");
    for(int i = 0; i < 100; i ++){
      if(newArgv[i]!=0){
        printf("%d %s", i, newArgv[i]);
      }
    exec(newArgv[0],newArgv);}// fork return 0 in child
    return;
  }
}

int
main(int argc, char *argv[])
{
  argBeg = 3;
  argEnd = argc;
  if(strcmp(argv[1],"-n")!=0 || strcmp(argv[2],"1")!=0){
    //printf("Wrong\n");
    //exit(0);
    argBeg = 1;
  }
  
  newArgv[0] = argv[0];
  for(int i = 0; i < argEnd-argBeg; i ++){
    newArgv[i] = argv[argBeg+i];
  }
  char buf[500];
  char* p = buf;
  char* q = buf;
  char curLine[500];
  char* ptr = buf;
  for(;;){
    int n = read(0,ptr,500);
    if(n==0) break;
    ptr += n;
  }
  //printf("In Buf:\n%s\n", buf);
  while(*q!='\0'){
    for(; *q!='\n' && *q!='\0' ; q++);
    memcpy(curLine, p , q-p);
    curLine[q-p] = '\0';
    p=q+1;
    q=q+1;
    //printf("curLine: %s\n",curLine);
    fire(curLine);
  }
  exit(0); 
}
