/*************************************************************

Sursa exemplificare fork, exec, sigaction

*************************************************************/
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// functia de tratare a semnalului
// folosim signatura extinsa pentru a obtine sursa semnalului

void sighandler (int sig, siginfo_t * structsig, void * ptr)
{
    if (sig == SIGUSR1)
    {
	// id-ul procesului ce a trimis semnalul
	// se regaseste in campul si_pid al structurii siginfo_t
	printf ("Received message from child %d \n", structsig->si_pid);
    }
}

int main(int argc, char * argv[])
{
    int i;
    struct sigaction act;
    
    // setam tratarea semnalului in cadrul procesului parinte
    memset(&act, 0 ,sizeof(struct sigaction));
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = sighandler;
    sigaction (SIGUSR1, &act, NULL);
    
    for (i=0; i<3; i++)
    {
	// cream trei procese copil
	pid_t pid = fork();
	
	if (pid == 0)
	{
	    // diferentiem codul pentru fiecare copil
	    // in functie de iteratie
	    switch(i)
	    {
		case 0: {
			// executam comanda externa ps in primul copil
			execlp("ps", "ps", NULL);
			
			}
			
		case 1: {
			
			// afisam id-ul procesului in al doilea copil
			pid_t mypid = getpid();
			printf ("1st child process has pid %d \n", mypid);
			
			// returnam un cod de eroare diferit de 0
			exit(9);
			}
			
		case 2: {
			// trimitem un semnal utilizator procesului parinte
			// in cadrul copilului trei
			pid_t ppid = getppid();
			kill (ppid, SIGUSR1); 
			exit(0);
			}
	    }
	}
    }
    
    for (i=0; i<3; i++)
    {
	int status;
	
	// obtinem starea de finalizare a fiecarui proces copil si extragem
	// codul de iesire
	pid_t child = wait(&status);
	printf ("Child %d terminated with status %d \n", child, WEXITSTATUS(status));
    }
    
    exit(0);
}
