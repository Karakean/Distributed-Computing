#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#define READ 0
#define WRITE 1
#define WORKERS 10

int semaphore [2];

void LOCK (){
    int x;
    read(semaphore[READ], &x ,sizeof(x));
}

void UNLOCK (){
    int x = 0;
    write(semaphore[WRITE], &x ,sizeof(x));
}

void init_semaphore(int num){
    for(int i=0; i < num; i++){
        UNLOCK();
    }
}

void worker (int id, int ending_pipe ){
    int x = 0, i, j;
    printf("Worker %d starts\n", id);
    LOCK();
    for ( i = 0; i < 7; i ++){
        printf ("[%d]",id);
        fflush(stdout);
        sleep(1);
    }
    UNLOCK();
    printf("Worker %d ends\n", id);
    write(ending_pipe, &x, sizeof(x));
}

int main (){
    int ending_pipe[2], i, x;
    pipe(ending_pipe);
    pipe(semaphore);
    init_semaphore(3);
    for (i = 0; i < WORKERS; i++){
        if (!fork()){
            worker (i, ending_pipe[WRITE]);
            return 0;
        }
    }
    for (i = 0; i < WORKERS; i++)
        read(ending_pipe[READ], &x, sizeof(x));
    puts ("end") ;
    return 0;
}