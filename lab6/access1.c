/* access1.c (vers. 1.0) */
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

extern int errno;

int main(int argc, char* argv[])
{
  int fd;
  char ch;

  if(argv[1] == NULL)
  {
    fprintf(stderr,"Trebuie apelat cu cel putin un parametru.\n");
    exit(1);
  }
  
  if( (fd=open("fis.dat", O_RDWR)) == -1)
  {  /* trateaza cazul de eroare ... */
    fprintf(stderr,"Nu pot deschide fisierul fis.dat -> error=%d : %s\n",errno, strerror(errno));
    exit(2);
  }

  /* parcurgerea fisierului caracter cu caracter pina la EOF */
  while( read(fd,&ch,1) != 0)
  {
    if(ch == '#')
    {
      lseek(fd,-1L,1);
      //sleep(1);
      write(fd,argv[1],1);
      printf("Terminat. S-a inlocuit primul char. # intilnit [ProcesID:%d].\n", getpid());
      return 0;
    }
  }
  
  printf("Terminat. Nu exista char. # in fisierul dat [ProcesID:%d].\n", getpid());
  return 0;
}
/* eof */
