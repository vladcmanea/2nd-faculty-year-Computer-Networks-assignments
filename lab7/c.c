/* cliTCPIt.c - Exemplu de client UDP
   Trimite un nume la server; primeste de la server "Hello nume".
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char msg[100];		// mesajul trimis
  int msglen=0,length=0;

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea dialogului cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  switch(fork())
  {
    case -1:
    {
	perror("[Client] Eroare la fork()");
	return errno;
    }
    case 0:
    {
	while ((msglen = recvfrom (sd, msg, 100, 0, (struct sockaddr*)&server, &length)) > 0)
	{
	    /* afisam mesajul primit */
	    write(1, msg, 100);
	}
	if (msglen == -1)
	{
	    perror("[Client] Eroare la recvfrom()");
	    return errno;
	}
	
	break;
    }
    default:
    {
	/* scriu cat am putut */
	while (1)
	{
	    bzero(msg, 100);
	    /* citesc sirul de intrare */
	    read(0, msg, 100);

	    /* trimit mesajul la server */
	    length = sizeof(server);
	    if (sendto (sd, msg, 100, 0, (struct sockaddr*)&server, length) <= 0)
	    {
    		perror ("[client]Eroare la sendto() spre server.\n");
    		return errno;
	    }
	}
	
	wait(NULL);
    }
  }
  
  /* inchidem conexiunea, am terminat */
  close (sd);
}
