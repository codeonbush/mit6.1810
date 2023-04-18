#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//格式化文件名
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

void
find(char *path, char *target){
    int fd;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_DEVICE:
        case T_FILE:
        //直接匹配文件名
            printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
            if (strcmp(fmtname(path), target) == 0){
                printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
            }
            break;
        case T_DIR:
            // while (/* condition */){
            //     /* code */
            // }
            
            // path = 0;
            // find(path, target);
            break;
    }
    close(fd);
    return;
}


int
main(int argc, char *argv[]){
    find(argv[1], argv[2]);
    exit(0);
}