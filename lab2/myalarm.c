
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define TIMER 3			/* temporizare la 3 sec. */

int counter = 0;

static void handler (int sig)			/* scrie numarul la iesirea standard */
{
  if (SIGALRM == sig)
  {
    counter++;
    printf ("%d %d\n", counter, getpid());
    if (counter >20)
	signal(SIGINT, SIG_DFL);
  }
  else
  if (sig == SIGUSR1)
  {
    FILE *f = fopen("myfile","w");
    fprintf (f,"Am primit un semnal");
    fclose(f);
  }
  else
  if (sig == SIGINT)
  {
  }
  
}

void
setsignals ()			
{
  signal(SIGALRM, handler);
  signal(SIGUSR1, handler);
  signal(SIGINT, handler);

  while (1)			/* rulam la infinit... */
    {
      alarm (TIMER);		/* setam o alta alarma */
      pause ();			/* asteptam aparitia unui semnal */
    }
}

int
main (void)			/* programul principal */
{

      setsignals ();

      return 0;
    
}
