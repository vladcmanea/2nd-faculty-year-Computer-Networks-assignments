/* access2.c (vers. 2.0) */
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

extern int errno;

int main(int argc, char* argv[])
{
  int fd,codblocaj;
  char ch;
  struct flock lacat;

  if(argv[1] == NULL)
  {
    fprintf(stderr,"Trebuie apelat cu cel putin un parametru.\n");
    exit(1);
  }

  if( (fd=open("fis.dat", O_RDWR)) == -1)
  {  /* trateaza cazul de eroare ... */
    perror("Nu pot deschide fisierul fis.dat deoarece ");
    exit(2);
  }

  /* pregateste lacat pe fisier */
  lacat.l_type   = F_WRLCK;
  lacat.l_whence = SEEK_SET;
  lacat.l_start  = 0;
  lacat.l_len    = 1; /* aici se poate pune orice valoare, inclusiv 0, deoarece
     pentru problema noastra nu conteaza lungimea zonei blocate. */

  /* Incercari repetate de punere a lacatului pina cind reuseste */
  while( ((codblocaj=fcntl(fd,F_SETLK,&lacat)) == -1)
         && ((errno==EACCES)||(errno==EAGAIN))         )
  { 
    fprintf(stderr, "Blocaj imposibil [ProcesID:%d].\n", getpid());
    perror("\tMotivul");
    sleep(6);
  }
  if(codblocaj == -1)
  {
    fprintf(stderr,"Eroare unknown la blocaj [ProcesID:%d].\n", getpid());
    perror("\tMotivul");
    exit(3);
  }
  else
    printf("Blocaj reusit [ProcesID:%d].\n", getpid());


  /* parcurgerea fisierului caracter cu caracter pina la EOF */
  while( read(fd,&ch,1) != 0)
  {
    if(ch == '#')
    {
      lseek(fd,-1L,1);
      sleep(10);
      write(fd,argv[1],1);
      printf("Terminat. S-a inlocuit primul char. # intilnit [ProcesID:%d].\n", getpid());
      return 0;
    }
  }
  
  printf("Terminat. Nu exista char. # in fisierul dat [ProcesID:%d].\n", getpid());
  return 0;
}
/* eof */
