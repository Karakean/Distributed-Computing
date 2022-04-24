#include <stdio.h>
#include <unistd.h>
int main(){
	int pid = fork();
	if (pid==-1){
		printf("Error while creating child process");
		return 1;
	}
	if (pid==0){
		printf("Child process 1: %d, parent: %d\n", getpid(), getppid());
		execl("./build.sh","build.sh", NULL);
	}
	else{
		printf("Waiting for process %d to end\n", pid);
		waitpid(pid, NULL, 0);
		printf("Process %d ended\n", pid);
		int pid2 = fork();
		if(pid2 == 0){
			printf("Child process 2: %d, parent: %d\n", getpid(), getppid()); 
			char* const env[2] = {"LD_LIBRARY_PATH=.", NULL};
                	execle("./binary.out", "binary.out", NULL, env);	
		}
		else {
			printf("Waiting for process %d to end\n", pid2);
			waitpid(pid2, NULL, 0);
			printf("Process %d ended\n", pid2);
			printf("Parent process: %d, children: %d and %d\n", getpid(), pid, pid2);
			execl("./delete.sh", "delete.sh", NULL);
		}
	}
	return 0;
}
