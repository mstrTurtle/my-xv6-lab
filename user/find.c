
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// targetName, 即待查找目标的名称.
char* targetName;

// 获取格式化名称, 从ls处摘抄.
char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

// 得到文件的名称
char*
getRegularName(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  
  return p;
}

// 递归地寻找文件
void
find(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

      

  switch(st.type){
  // 当传入参数path为regular file, 则对比, 打印, 并返回.
  case T_FILE:
    if(strcmp(getRegularName(path), targetName)==0)
      printf("%s\n", path); // FOUND IT!
    break;

  // 当传入参数path为目录, 则递归地为每个directory entry执行find
  case T_DIR:
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){

      if(de.inum == 0)
        continue;
      if(strcmp(getRegularName(de.name),".")==0 || strcmp(getRegularName(de.name),"..")==0)
        continue;

      
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      
      find(buf);
    }
    break;
  }
  close(fd);
}

// Write a simple version of the UNIX find program: 
// find all the files in a directory tree with a specific name. 
// Your solution should be in the file user/find.c.
int
main(int argc, char *argv[])
{
  targetName = argv[2];
  find(argv[1]);
  exit(0); 
}
