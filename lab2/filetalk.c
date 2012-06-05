
#include <stdio.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sighandler (int sig)
{
    if (sig == SIGUSR1)
    {
	FILE *f;
	char line[100];
	f = fopen("file","r");
	fgets(line, 100, f);
	printf ("Procesul copil cu pid %d a primit mesajul %s", getpid(), line);
	fclose(f);
    }
}

int main ()
{
    pid_t pid;
    FILE * f; 
    
  pid = fork();
  if (pid < 0)
    {
      perror ("Eroare la fork()");
      return (1);
    }
  else
  if (pid == 0)
  {
    
    signal (SIGUSR1, sighandler);
    
    pause();
    
    return 0;    
  }
  else
  {
    if (pid % 2 == 0)
    {
        printf ("Notificam procesul fiu cu pid %d", pid);
	f = fopen ("file","w");
        fprintf (f,"Good morning!");
	fclose(f);
	kill (pid, SIGUSR1);
    }
    else 
    {
	printf ("Notificam procesul fiu cu pid %d", pid);
	f = fopen ("file","w");
	fprintf (f,"Good night!");
	fclose(f);
	kill (pid, SIGUSR1);
    }
  }

  wait(NULL);
  return 0;
}
