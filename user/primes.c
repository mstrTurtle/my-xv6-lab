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
// Design: algorithm as below...
// ::::::::::::::::::::::::::::::::::
//     int N = 35;
//     int n = 2;
//     int ps[2];
//     int qs[2];
//     int in, out;
// ent:
//     in, out = ps2,qs1;

//     if(n<N && fork == child){ // &&是用来短路用的.
//         in, out = qs2,rs1;
//         ps, qs = qs, rs;
//         rs = createNewPipe;
//         n = read(in)
//         print n;
        
//         while(xx=read(in)){
//             if xx%n:
//                 drop
//             else:
//                 write(out)
//         }
//         goto ent;
//     }else{
//         exit
//     }

// implement:
    int N = 35;
    int n = 2;
    int ps[2];
    int qs[2];
    int rs[2];
    int in, out, nextIn;
    int buffer;
    pipe(ps);
    pipe(qs);
    pipe(rs);
    
    in=ps[0];out = qs[1]; nextIn = qs[0];
    printf("prime %d\n", 2);
    for(int i = 3; i <= 100; i+=2){
        write(out,(void*)&i,sizeof(i));
    }
    close(in);//回收
ent:                                                                    //ent
    //in = ps[0]; out = qs[1];                                          //in, out = psR,qsW;

    if(n<N&&fork()>0){                                                  //if(n<N && fork == child){ // &&是用来短路用的.
        close(out);//惯例.子进程关父进程out
        //printf("pid: %d\n",getpid());
        in = qs[0];out=rs[1];nextIn=rs[0];                              //  in, out = qsR,rsW;
        ps[0]=qs[0];ps[1]=qs[1];
        qs[0]=rs[0];qs[1]=rs[1];                                        //  ps, qs = qs, rs;
        pipe(rs);                                                       //  rs = createNewPipe;
        if(read(in,(void*)&buffer,sizeof(int))){ n = buffer;}           //  n = read(in)
        else{exit(0);}
        printf("prime %d\n",n);                                         //  print n;
        int ret;
        while((ret=read(in,(void*)&buffer,sizeof(int)))){               //  while(xx=read(in)){
            if(buffer%n==0){                                            //      if xx%n:
             }                                                          //          drop
            else{                                                       //      else:
                write(out,(void*)&buffer,sizeof(int));}                 //          write(out)
        }                                                               //  }
        //close(in);close(out);//回收.每个进程死之前关闭in和out.
        goto ent;                                                       //  goto ent;
    }else{                                                              //}else{
        close(nextIn);//惯例.父进程关子进程in
        exit(0);                                                        //  exit
    }                                                                   //}

}
