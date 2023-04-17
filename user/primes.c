#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXSIZE  35

void
sleve_num(int *pl){
    int pr[2];
    int num;
    int prime;
    
    if (read(pl[0], &num, sizeof(int)) > 0){
        prime = num;
        printf("prime %d\n", prime);
        pipe(pr);

        if(fork() == 0){
            sleve_num(pr);
        }else{
            close(pr[0]);
            while (read(pl[0], &num, sizeof(int)) > 0){
                if(num % prime != 0){
                    write(pr[1], &num, sizeof(int));
                }
            }
            close(pl[0]);
            close(pr[1]);
            wait(0);
        }
    }
    exit(0);
}

int
main(int argc, char *argv[]){
    int p[2];
    pipe(p);
    for (int i = 2; i < MAXSIZE; i++){
        write(p[1], &i, sizeof(int));
    }
    close(p[1]);
    sleve_num(p);
    wait(0);
    exit(0);
}

