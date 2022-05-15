#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#define READ 0
#define WRITE 1
#define MAX_STRING_SIZE 30
int main ()
{
    if (fork()){
        char str[MAX_STRING_SIZE];
        printf("Type a string: ");
        scanf("%s",str);
        int f1 = open("fifo1", O_WRONLY);
        write(f1, str ,sizeof(str));
        close(f1);
    }
    else{
        if(fork()){
            char str[MAX_STRING_SIZE];
            char new_str[MAX_STRING_SIZE];
            int f1 = open("fifo1", O_RDONLY);
            read(f1, str, sizeof(str));
            //printf("Second process received: %s\n", str);
            close(f1);
            for(int i = 0; i < strlen(str); i++){
                new_str[i] = str[i];
                if (str[i] > 'a' && str[i] < 'z')
                    new_str[i] -= 0x20;
            }
            int f2 = open("fifo2", O_WRONLY);
            write(f2, new_str, sizeof(new_str));
            //printf("Second process sent: %s\n", new_str);
            close(f2);
        }
        else{
            char str[MAX_STRING_SIZE];
            int f2 = open("fifo2", O_RDONLY);
            read(f2, str, sizeof(str));
            printf("Third process received: %s\n", str);
            close(f2);
        }
    }
    return 0;
}