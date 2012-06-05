/* Client TCP ce solicita rezolvarea unei operatii aritmetice de catre server
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

struct operation
{
    int number1;
    int number2;
    char op;
};

struct response
{
    char message[30];
    int result;
};

/* portul de conectare la server*/
int port;

/* programul */
int
main (int argc, char *argv[])
{
  int sd;			/* descriptorul de socket */
  struct sockaddr_in server;	/* structura folosita pentru conectare */
  struct operation request;	/* cererea trimisa clientului */
  struct response feedback;

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  server.sin_family = AF_INET;
  		/* familia socket-ului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
		/* adresa IP a serverului */
  server.sin_port = htons (port);
  		/* portul de conectare */

  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,
	       sizeof (struct sockaddr)) == -1)
    {
      perror ("Eroare la connect().\n");
      return errno;
    }

  /* citirea mesajului si trimiterea catre server */
  bzero (&request, sizeof(struct operation));
  printf ("Introduceti operatia: ");
  scanf ("%d %c %d", &request.number1, &request.op, &request.number2);
  fflush (stdout);
  
  request.number1 = htonl(request.number1);
  request.number2 = htonl(request.number2);

  if (send (sd, &request, sizeof(struct operation), 0) <= 0)
    {
      perror ("Eroare la write() spre server.\n");
      return errno;
    }

  /* citirea raspunsului dat de server 
     (ne blocam pina cind serverul raspunde) */
  if (recv (sd, &feedback, sizeof(struct response), 0) < 0)
    {
      perror ("Eroare la read() de la server.\n");
      return errno;
    }
  
  /* afisam mesajul primit */
  printf ("%s %d\n", feedback.message,ntohl(feedback.result));

  /* inchidem conexiunea, am terminat */
  close (sd);
}
