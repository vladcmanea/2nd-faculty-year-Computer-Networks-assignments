/* 
   Furnizare serviciu echo la doua porturi distincte
   prin utilizarea multiplexarii select
   (adaptare exemplu curs)
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

/* portul folosit */

#define PORTS1 19997
#define PORTS2 19999

extern int errno;		/* eroarea returnata de unele apeluri */

/* functie de convertire a adresei IP a clientului in sir de caractere */
char *
conv_addr (struct sockaddr_in address)
{
  static char str[25];
  char port[7];

  /* adresa IP a clientului */
  strcpy (str, inet_ntoa (address.sin_addr));	
  /* portul utilizat de client */
  bzero (port, 7);
  sprintf (port, ":%d", ntohs (address.sin_port));	
  strcat (str, port);
  return (str);
}

/* programul */
int main (int argc, char * argv[])
{
  struct sockaddr_in server;	/* structurile pentru server si clienti */
  struct sockaddr_in from;
  fd_set readfds;		/* multimea descriptorilor de citire */
  struct timeval tv;		/* structura de timp pentru select() */
  int sd1, sd2, client;		/* descriptori de socket */
  int fd;			/* descriptor folosit pentru 
				   parcurgerea listelor de descriptori */
  int nfds;			/* numarul maxim de descriptori */
  int len;			/* lungimea structurii sockaddr_in */

  int ports1;
  int ports2;
  
  /* completarea porturilor */
  if (argc == 3)
  {
    ports1 = atoi(argv[1]);
    ports2 = atoi(argv[2]);    
  }
  else
  {
    ports1 = PORTS1;
    ports2 = PORTS2;
  }
  
  /* creare socketuri */
  if ((sd1 = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  if ((sd2 = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }
    
  /* pregatim structurile de date */
  bzero (&server, sizeof (server));

  /* umplem structura folosita de server */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (ports1);

  /* atasam socketurile */
  if (bind (sd1, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("Eroare la bind().\n");
      return errno;
    }
    
  server.sin_port = htons (ports2);

  /* atasam socketul */
  if (bind (sd2, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze la primul port*/
  if (listen (sd1, 5) == -1)
    {
      perror ("Eroare la listen().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze la al doilea port*/
  if (listen (sd2, 5) == -1)
    {
      perror ("Eroare la listen().\n");
      return errno;
    }
      
  /* completam multimea de descriptori de citire */
  FD_ZERO (&readfds);		/* initial, multimea este vida */

  tv.tv_sec = 1;		/* se va astepta un timp de 1 sec. */
  tv.tv_usec = 0;
  
  /* valoarea maxima a descriptorilor folositi */
  nfds = (sd1 > sd2) ? sd1 : sd2 ;

  printf ("Asteptam la porturile %d %d...\n", ports1, ports2);
  fflush (stdout);

  int children = 0;
          
  /* utilizarea multiplexarii select pentru a furniza acelasi serviciu la porturi distincte */
  while (1)
    {
      
      FD_SET (sd1, &readfds);	/* includem in multime socketurile create */
      FD_SET (sd2, &readfds);

      /* apelul select() */
      if (select (nfds+1, &readfds, NULL, NULL, &tv) < 0)
	{
	  perror ("Eroare la select().\n");
	  return errno;
	}
	
      /* vedem daca e pregatit socketul pentru a-i accepta pe clienti
         (daca avem cereri pe primul port) */
      if (FD_ISSET (sd1, &readfds))
	{
	  printf ("Cerere primita la portul %d \n", ports1);
	  fflush(stdout);
	  
	  /* pregatirea structurii client */
	  len = sizeof (from);
	  bzero (&from, sizeof (from));

	  /* a venit un client, acceptam conexiunea */
	  client = accept (sd1, (struct sockaddr *) &from, &len);
	  
	  /* eroare la acceptarea conexiunii de la un client */
	  if (client < 0)
	    {
	      perror ("Eroare la accept().\n");
	      continue;
	    }
	
	  printf
	    ("S-a conectat clientul cu descriptorul %d, de la adresa %s.\n",
	      client, conv_addr (from));
	  fflush(stdout);
	  handleEchoClient(client);
	}
      
      /* aceleasi operatii pentru al doilea port */
      if (FD_ISSET (sd2, &readfds))
	{
	  printf ("Cerere primita la portul %d \n", ports2);
	  fflush(stdout);
	  
	  /* pregatirea structurii client */
	  len = sizeof (from);
	  bzero (&from, sizeof (from));

	  /* a venit un client, acceptam conexiunea */
	  client = accept (sd2, (struct sockaddr *) &from, &len);
	  
	  /* eroare la acceptarea conexiunii de la un client */
	  if (client < 0)
	    {
	      perror ("Eroare la accept().\n");
	      continue;
	    }
	
	  printf
	    ("S-a conectat clientul cu descriptorul %d, de la adresa %s.\n",
	      client, conv_addr (from));
	  fflush(stdout);
	  handleEchoClient(client);
	}	
    }				/* while */
}				/* main */

/* realizeaza primirea si retrimiterea unui mesaj unui client */
int
handleEchoClient (int fd)
{
  char buffer[100];		/* mesajul */
  int bytes;			/* numarul de octeti cititi/scrisi */

  bytes = read (fd, buffer, sizeof (buffer));
  if (bytes < 0)
    {
      perror ("Eroare la read() de la client.\n");
      return 0;
    }
  if (bytes && write (fd, buffer, bytes) < 0)
    {
      perror ("Eroare la write() catre client.\n");
      return 0;
    }
  
  return bytes;
}
