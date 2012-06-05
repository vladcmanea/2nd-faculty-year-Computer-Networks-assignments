/* Server TCP ce rezolva o cerere constand intr-o operatie aritmetica
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* portul folosit */
#define PORT 42000

struct operation
{
    int number1;
    int number2;
    char op;
};

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* programul */
int main (int argc, char * argv[])
{
  struct sockaddr_in server;	/* structurile folosite de server si client */
  struct sockaddr_in from;
  struct operation request;	/* cererea primita de server */
  int sd;			/* descriptorul de socket */
  int port;
  
  /* setare port in cazul specificarii explicite */
  if (argc > 1)
  {
    port = atoi(argv[1]);
  }
  else
    port = PORT;

  /* cream un socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* pregatim structurile de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  server.sin_family = AF_INET;	
  	/* stabilirea familiei de socket-uri */
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  	/* acceptam orice adresa */
  server.sin_port = htons (port);
  	/* utilizam un port utilizator */

  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)
    {
      perror ("Eroare la listen().\n");
      return errno;
    }

  /* servim in mod iterativ clientii... */
  while (1)
    {
      int client;
      int length = sizeof (from);

      printf ("Asteptam la portul %d...\n", port);
      fflush (stdout);

      /* acceptam un client (ne vom bloca pina la realizarea conexiunii) */
      client = accept (sd, (struct sockaddr *) &from, &length);

      /* eroare la acceptarea conexiunii de la un client */
      if (client < 0)
	{
	  perror ("Eroare la accept().\n");
	  continue;
	}

      /* am realizat conexiunea, asteptam mesajul... */
      bzero (&request, sizeof(struct operation));
      printf ("Asteptam cererea...\n");
      fflush (stdout);
      
      /* citirea mesajului */
      int nrbytes = recv (client, &request, sizeof(struct operation), 0);
      if (nrbytes <= 0)
	{
	  perror ("Eroare la read() de la client.\n");
	  close (client);	/* inchidem conexiunea cu clientul */
	  continue;		/* continuam sa ascultam... */
	}

      if (nrbytes < sizeof(struct operation))
      {
        perror ("Date primite incomplet");
	close (client);
	continue;
      }
      
      request.number1 = ntohl(request.number1);
      request.number2 = ntohl(request.number2);
      
      printf ("Cerere primita... \n In curs de rezolvare: %d %c %d \n", 
    	       request.number1, request.op, request.number2);
      
      /* rezolvarea cererii */
      int result = 0;
      
      switch(request.op)
      {
        case '+': result = request.number1 + request.number2;
		  break;
	case '-': result = request.number1 - request.number2;
		  break;
	case '*': result = request.number1 * request.number2;
		  break;
	case '/': result = request.number1 / request.number2;
		  break;
      }
      
      result = htonl(result);

      /* returnam rapunsul clientului */
      if (send (client, &result, sizeof(int),0) <= 0)
	{
	  perror ("Eroare la write() catre client.\n");
	  continue;		/* continuam sa ascultam */
	}
      else
	printf (" trasmitere cu succes.\n");
      /* am terminat cu acest client, inchidem conexiunea */
      close (client);
    }				/* while */
}				/* main */
