#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

void
catgrep () 
{
   
  mknod ("MyFIFO", S_IFIFO | 0666, 0);
  int fd; 
  /* cream pipe-ul */
  /* cream primul copil */  
  switch (fork ())              
    {
    case -1:
      fprintf (stderr, "fork - 1\n");
      exit (1);
    case 0:                        /* copilul */
      fd = open ("MyFIFO", O_WRONLY);
      close (1);
      /* duplicam descriptorul pipe-ului
         la iesirea standard (1) */ 
      if (dup (fd) != 1)    
        {
          fprintf (stderr, "dup - 1\n");
          exit (1);
        }
      
      close (fd);
      
      execlp ("cat", "cat", "prog.c", NULL);
      fprintf (stderr, "exec - 1\n");
      exit (1);
    }
  /* cream al doilea copil */  
  switch (fork ())
    {
    case -1:
      fprintf (stderr, "fork - 2\n");
      exit (1);
    case 0:                        /* copilul */
      fd = open ("MyFIFO", O_RDONLY);
      close (0);
      /* duplicam descriptorul pipe-ului
         care va fi asociat intrarii standard */
      if (dup (fd) != 0)
        {
          fprintf (stderr, "dup - 2\n");
          exit (1);
        }
      
      close (fd);
      
      execlp ("grep", "grep", "include", NULL);
      fprintf (stderr, "exec - 2\n");
      exit (1);
    }
  /* parintele */
  /* inchidem pipe-ul, nu-l folosim deloc */
  /* asteptam terminarea copiilor */
  while (wait (NULL) != -1)
    ;
}

int
main () /* programul principal */
{
  catgrep ();
  return 0;
} 