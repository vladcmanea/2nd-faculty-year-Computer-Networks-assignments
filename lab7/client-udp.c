/* Client UDP ce trimite cereri de rezolvare operatii aritmetice catre un server
   (adaptare dupa exemplul 'echo' din pagina cursului)
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

/* structura pentru memorarea datelor specifice operatiei */
struct operation
{
    int number1;  //operand 1
    int number2;  //operand 2
    char op;      //opcode - '+','-','*' sau '/'
 };

/* programul */
int
main (int argc, char *argv[])
{
  int sd;			/* descriptorul de socket */
  struct sockaddr_in server;	/* structura folosita pentru conectare */
  struct operation request;	/* structura trimisa */
  int length;

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

  /* afisam ca date informative dimensiunile bufferelor de trimitere si primire */
  int value;
  int valuesize=sizeof(int);
  
  getsockopt(sd, SOL_SOCKET, SO_RCVBUF, &value, &valuesize);
  printf("Dimensiunea bufferului de primire este %d\n", value);
  
  getsockopt(sd, SOL_SOCKET, SO_SNDBUF, &value, &valuesize);
  printf("Dimensiunea bufferului de trimitere este %d\n", value);

  /* incepem sa trimitem cereri */
  while (1)
  {
  
  /* umplem structura folosita pentru realizarea dialogului cu serverul */
  server.sin_family = AF_INET;
  	/* familia socket-ului */
  server.sin_addr.s_addr = inet_addr (argv[1]);
  	/* adresa IP a serverului */
  server.sin_port = htons (port);
  	/* portul de conectare */

  /* citirea operatiei si trimiterea catre server */
    bzero (&request, sizeof(struct operation));
    printf ("Introduceti operatia:"); 
    scanf ("%d %c %d", &request.number1, &request.op, &request.number2);	    
    request.number1 = htonl(request.number1);
    request.number2 = htonl(request.number2);	      

  length = sizeof (server);
  /* trimiterea operatiei catre server */
  if (sendto (sd, &request, sizeof(request), 0, 
              (struct sockaddr *) &server, length) < 0)
    {
      perror ("Eroare la sendto() spre server.\n");
      return errno;
    }
    
  int result;
  /* citirea raspunsului dat de server
     (ne blocam pina cind serverul raspunde) */
  if (recvfrom (sd, &result, sizeof(int), 0, 
      (struct sockaddr *) &server, &length) < 0)
    {
      perror ("Eroare la recvfrom() de la server.\n");
      return errno;
    }
    
  printf ("Resultatul primit este: %d\n", result);
  
  /* oferim optiuni utilizatorului */
  char option;
  printf ("Continui, renunt sau schimb serverul ? (c/r/s) \n");
  
  scanf ("%c %c",&option, &option);

  switch(option)
  {
    /* continuam trimiterea de operatii spre acelasi server */
    case 'c':break;
    /* iesim din program */
    case 'r':close (sd);
	     exit(0);
	     break;
    /* schimbam adresa serverului inainte de a introduce urmatoarea operatie */
    case 's':printf("Introduceti o noua adresa IP pentru server:");
	     scanf("%s",argv[1]);
	     printf("Introduceti un nou port pentru server:");
	     scanf("%d",&port);
	     break;
  }
  }
}
