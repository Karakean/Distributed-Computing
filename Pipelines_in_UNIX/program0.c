#include <unistd.h>
#include <stdio.h>
#define READ 0
#define WRITE 1
int main ()
{
    int pipe_handle[2];
    int x;
    puts("Start of a program");
    puts("Creating pipe");
    pipe(pipe_handle);
    puts("Forking");
    if (fork()){
        puts("Parent process:");
        close(pipe_handle[WRITE]);
        puts("Waiting for child process...");
        read(pipe_handle[READ], &x, sizeof(x));
        printf("Parent process received %d\n", x);
        close(pipe_handle[READ]);
    }
    else{
        puts("Child process");
        close(pipe_handle[READ]);
        x = 10;
        sleep(10);
        write(pipe_handle[WRITE], &x ,sizeof(x));
        printf("Child process sent %d\n", x);
        close(pipe_handle[WRITE]);
    }
    puts("End of a program");
    return 0;
}