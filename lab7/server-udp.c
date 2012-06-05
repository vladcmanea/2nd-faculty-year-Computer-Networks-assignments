/* Server UDP iterativ ce trateaza cereri de rezolvari de operatii aritmetice
   (adaptare dupa serverul UDP din pagina cursului)
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

/* portul folosit */
#define PORT 42000

/* structura operatiei */
struct operation
{
   int number1;	 //operand 1
   int number2;  //operand 2
   char op;      //opcode - '+','-','*' sau '/'
};
	 

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* programul */
int
main (int argc, char* argv[])
{
  struct sockaddr_in adresa;	/* structurile folosite de server si client */
  struct sockaddr client;
  struct operation request;	/* cererea trimisa de client */
  int sd;			/* descriptorul de socket */
  int port;
  
  /* completam portul din argumentele programului */
  if (argc > 1)
  {
    port = atoi(argv[1]);
  }
  else
    port = PORT;

  /* lansam serverul in fundal... */
  switch (fork ())
    {
    case -1:			/* eroare la fork() */
      perror ("Fork error\n");
      return errno;
    case 0:			/* copilul traieste... */
      break;
    default:			/* parintele moare... */
      printf ("Serverul se lanseaza in fundal...\n");
      exit (0);
    }

  /* cream un socket */
  if ((sd = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* pregatim structura folosita de server */
  adresa.sin_family = AF_INET;
  	/* stabilirea familiei de socket-uri */
  adresa.sin_addr.s_addr = htonl (INADDR_ANY);
  	/* acceptam orice adresa */
  adresa.sin_port = htons (port);
  	/* utilizam un port utilizator */

  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &adresa, sizeof (struct sockaddr)) == -1)
    {
      perror ("Eroare la bind().\n");
      return errno;
    }

  /* servim in mod iterativ clientii... */
  while (1)
    {
      int bytes;
      int length = sizeof (client);

      /* citim cererea primita de la client */
      if ((bytes = recvfrom (sd, &request, sizeof(struct operation), 0, 
                   (struct sockaddr *) &client, &length)) < 0)
	{
	  perror ("Eroare la recvfrom() de la client.\n");
	  continue;
	}

      if (bytes < sizeof(struct operation))
      {
	perror ("Date primite incomplet");
	continue;
      }
						            
      request.number1 = ntohl(request.number1);
      request.number2 = ntohl(request.number2);
      
      printf ("Cerere primita la portul %d... \n In curs de rezolvare: %d %c %d \n", 
        	port, request.number1, request.op, request.number2);

      /* facem calculele */		         
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
			      
      result = htonl(result);		      			 															          result = htonl(result);
																					  
      /* ...dupa care trimitem rezultatul inapoi */
      if (sendto (sd, &result, sizeof(int), 0, 
                  (struct sockaddr *) &client, length) < 0)
	{
	  perror ("Eroare la sendto() spre client.\n");
	  return errno;
	}
    }				/* while */
}				/* main */
