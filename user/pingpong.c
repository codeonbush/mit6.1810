#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

  int p[2];
  char buf[512];
  pipe(p);

  if (fork() == 0){
    if(read(p[0], buf, 5) > 0){
        write(1, buf, 5);
    }
    close(p[0]);
    write(p[1], "pong\n", 5);
    close(p[1]);
  }else{
    write(p[1], "ping\n", 5);
    close(p[1]);
    if (read(p[0], buf, 5);)
    {
       write(1, buf, 5);
    }
    close(p[0]);
  }
  exit(0);
}