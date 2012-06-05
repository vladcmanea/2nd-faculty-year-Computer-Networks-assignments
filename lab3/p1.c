#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    int pfd1[2];
    int pfd2[2];
    int x, n;

    if (pipe(pfd1) == -1)
    {
	// eroare
	return 0;
    }

    if (pipe(pfd2) == -1)
    {
	// eroare
	return 0;
    }

    switch(fork())
    {
	case -1:
	{
	    return 0;
	}
	case 0:
	{
	    close(pfd1[1]);
	    read(pfd1[0], &n, 4);

	    close(pfd2[0]);
	    n %= 2;
	    write(pfd2[1], &n, 4);
	    
	    break;
	}
	default:
	{
	    printf("N=");
	    scanf("%d", &n);
	
	    close(pfd1[0]);
	    write(pfd1[1], &n, 4);
	    
	    close(pfd2[1]);
	    read(pfd2[0], &n, 4);

	    printf("Am primit raspuns %d \n", n);
	    
	    wait(NULL);
	}
    }

    return 0;
}