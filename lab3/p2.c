#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO "hello"

int main()
{
    int fd;

    mknod(FIFO, S_IFIFO|0666, 0);
    
    switch(fork())
    {
	case -1:
	{
	    // error
	    return 0;
	}
	case 0:
	{
    	    fd = open(FIFO, O_WRONLY);
	    close(1);
	    if (dup(fd) != 1)
	    {
		// error
		return 0;
	    }
	    execlp("cat", "cat", "prog.c", NULL);
	}
	default:
	{
	    // sunt in parinte
	    fd = open(FIFO, O_RDONLY);
	    close(0);
	    if (dup(fd) != 0)
	    {
		// error
		return 0;
	    }
	    execlp("grep", "grep", "include", NULL);
	    
	    wait(NULL);
	}
    }
    
    return 0;
}