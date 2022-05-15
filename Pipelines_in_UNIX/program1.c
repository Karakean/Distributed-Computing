#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define READ 0
#define WRITE 1
#define MAX_STRING_SIZE 30
int main ()
{
    int pipe_handles_12[2];
    pipe(pipe_handles_12);
    if (fork()){
        close(pipe_handles_12[READ]);
        char str[MAX_STRING_SIZE];
        printf("Type a string: ");
        scanf("%s",str);
        write(pipe_handles_12[WRITE], str ,sizeof(str));
        //printf("First process sent: %s\n", str);
        close(pipe_handles_12[WRITE]);
    }
    else{
        int pipe_handles_23[2];
        pipe(pipe_handles_23);
        if(fork()){
            close(pipe_handles_23[READ]);
            close(pipe_handles_12[WRITE]);
            char str[MAX_STRING_SIZE];
            char new_str[MAX_STRING_SIZE];
            read(pipe_handles_12[READ], str, sizeof(str));
            //printf("Second process received: %s\n", str);
            close(pipe_handles_12[READ]);
            for(int i = 0; i < strlen(str); i++){
                new_str[i] = str[i];
                if (str[i] > 'a' && str[i] < 'z')
                    new_str[i] -= 0x20;
            }
            write(pipe_handles_23[WRITE], new_str, sizeof(new_str));
            //printf("Second process sent: %s\n", new_str);
            close(pipe_handles_23[WRITE]);
        }
        else{
            char str[MAX_STRING_SIZE];
            close(pipe_handles_12[READ]);
            close(pipe_handles_12[WRITE]);
            close(pipe_handles_23[WRITE]);
            read(pipe_handles_23[READ], str, sizeof(str));
            printf("Third process received: %s\n", str);
            close(pipe_handles_23[READ]);
        }
    }
    return 0;
}