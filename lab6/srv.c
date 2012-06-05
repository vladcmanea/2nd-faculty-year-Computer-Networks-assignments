/* servTCPIt.c - Exemplu de server TCP iterativ
   Asteapta un nume de la clienti; intoarce clientului sirul
   "Hello nume".
   
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* portul folosit */
#define PORT 1668
/* codul de eroare returnat de anumite apeluri */
extern int errno;
int i, k;
char *p, v0[10000], v[10][1000];

int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	

  char msg[100];		//mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client

  int sd;			//descriptorul de socket 
  int children;
  
  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
    
  /* set sock option */
  // setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  
  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }

  /* servim in mod concurent clientii... */
  while (1)
    {
      int client, child;
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);
      
      children = 0;

      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      client = accept (sd, (struct sockaddr *) &from, &length);

      /* eroare la acceptarea conexiunii de la un client */
      if (client < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}
      
      /* aici fac un fork */
      switch(fork())
      {
        case -1:
        {
    	    perror("[server]Eroare la fork() de la server.\n");
    	    break;
        }
        case 0:
        {
    	    /* sunt in copil, fac cererea */
    	    //close(sd);
    	    
          /* s-a realizat conexiunea, se astepta mesajul */
	    bzero (msg, 100);
    	    printf ("[server]Asteptam mesajul...\n");
    	    fflush (stdout);
      
    	    /* citirea mesajului */
    	    if (read (client, msg, 100) <= 0)
	    {
		perror ("[server]Eroare la read() de la client.\n");
		close (client);	/* inchidem conexiunea cu clientul */
		continue;		/* continuam sa ascultam */
	    }
	
    	    /* creez un copil */
    	    switch(child = fork())
    	    {
    		case -1:
    		{
        	    perror("[Server]Eroare la fork().\n");
    		    break;
    		}
    		case 0:
    		{
    		    /* inchid iesirea standard */
    		    close(1);

    		    if (dup(client) == -1)
    		    {
    			perror("[server]Eroare la dup().\n");
    			continue;
    		    }
    		    
    		    for (i = 0; msg[i] != '\n'; ++i);
    		    msg[i] = 0;
    		    execlp(msg, msg, NULL);
    		}
    		default:
    		{
    		    /* sunt in parinte, nu mai fac nimic... */
    		    wait(NULL);
    		    close(client);
    		}
    	    }
    	    
    	    close(client);
    	    exit(0);
    	}
    	default:
    	{
    	    children++;
    	    
    	    /* sunt in parinte... */
    	    close(client);
    	    
    	    /* fac curatenie */
    	    while (children)
    		if (waitpid(-1, NULL, WNOHANG) != 0)
    		    children--;
    		else
    		    break;
    	}
      }
    }				/* while */
}				/* main */
