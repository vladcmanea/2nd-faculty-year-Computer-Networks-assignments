
// TODO: add error detection

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
    int pipe1[2];	//pipe pe directia parinte - fiu
    int pipe2[2];	//pipe pe directia fiu - parinte
    
    int nr;
    char answer[4];

    pipe(pipe1);
    pipe(pipe2);
    
    int pid = fork();
    
    if (pid)
    {
	close(pipe1[0]);
	close(pipe2[1]);
	
	scanf("%d",&nr);
	
	write(pipe1[1], &nr, sizeof(int));
	read(pipe2[0], answer, 4);
	
	if (strcmp(answer,"yes")==0)
	    printf("The number is even\n");
	else
	    printf("The number is odd\n");
	    
	close(pipe1[1]);
	close(pipe2[0]);
	
	wait(NULL);
    }
    else
    {
	close(pipe1[1]);
	close(pipe2[0]);
	
	read(pipe1[0], &nr, sizeof(int));
	
	if (nr % 2)
	    strcpy(answer,"no");
	else
	    strcpy(answer,"yes");
	    
	write(pipe2[1],answer,4);
	
	close(pipe1[0]);
	close(pipe2[1]);
    }
    
    return 0;
}
