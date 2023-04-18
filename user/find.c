#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
rtrim(char* path)
{
    static char newStr[DIRSIZ+1];
    int whiteSpaceSize = 0;
    int bufSize = 0;
    for(char* p = path + strlen(path) - 1; p >= path && *p == ' '; --p) {
        ++whiteSpaceSize;
    }
    bufSize = DIRSIZ - whiteSpaceSize;
    memmove(newStr, path, bufSize);
    newStr[bufSize] = '\0';
    return newStr;
}

//filepath to filename
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
  return rtrim(buf);
}

void
find(char *path, char *target){
    int fd;
    char buf[512], *p;
    struct dirent de;
    struct stat st;
    char *filename;
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
            filename = fmtname(path);
            printf("path: %s, target: %s,  filename: %s, cmp: %d \n", path, target, filename, strcmp(fmtname(path), target));
            if (strcmp(filename, target) == 0){
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            //第一个+1为“/”，第二个+1为 0
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                continue;
                /**
                 * @brief The memmove() function is used to copy DIRSIZ bytes of data
                 * from the memory location pointed to by de.name to the memory location pointed to by p.
                 * 
                 */
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                printf("pathbuf: %s \n", buf);
            }
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