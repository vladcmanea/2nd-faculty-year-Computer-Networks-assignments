/* servTCPIt.c - Exemplu de server UDP
   Asteapta un nume de la clienti; intoarce clientului sirul
   "Hello nume".
   
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
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
#define PORT 2778

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* vector de clienti */

struct cli
{
  char nickname[100];
  struct sockaddr_in addr;
} V[100];

char vec1[100], vec2[100], vec3[100];
int count, k, j, i;

int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in client;	
  char msg[100];		//mesajul primit de la client 
  char msgrasp[100] = " ";      //mesaj de raspuns pentru client
  
  int sd, opt;			//descriptorul de socket 

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }

  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&client, sizeof (client));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
    
    opt = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }
  
  /* servim in mod iterativ clientii... */
  while (1)
    {
      int msglen;
      int length = sizeof (client);

      printf ("[server]Astept la portul %d...\n",PORT);
      fflush (stdout);

      bzero (msg, 100);
      
      /* citirea mesajului primit de la client */
      if ((msglen = recvfrom(sd, msg, 100, 0,(struct sockaddr*) &client, &length)) <= 0)
	{
	  perror ("[server]Eroare la recvfrom() de la client.\n");
	  return errno;
	}
		
      if (strncmp(msg, "login", 5) == 0)
      {
        // actiune de login
        for (k = 5; msg[k] == ' '; ++k);
        for (j = 0; 'a' <= msg[k] && msg[k] <= 'z'; ++k, ++j)
    	   vec1[j] = msg[k];
    	vec1[j] = 0;
        
        for (i = 0; i < count && strcmp(vec1, V[i].nickname) != 0; ++i);
        if (i == count)
        {
    	    // n-am gasit, il pun
    	    ++count;
    	    strcpy(V[count].nickname, vec1);
    	    V[count].addr = client;
    	}
    	else
    	{
    	    // l-am gasit
    	    bzero(msgrasp, 100);
    	    strcpy(msgrasp, "Already logged in with name ");
    	    strcat(msgrasp, V[i].nickname);
    	    strcat(msgrasp, "\n");
      
    	    /* returnam mesajul clientului */
    	    if (sendto(sd, msgrasp, 100, 0, (struct sockaddr*) &client, length) <= 0)
	    {
		perror ("[server]Eroare la sendto() catre client.\n");
		continue;		/* continuam sa ascultam */
	    }
    	    else
		printf ("[server]Mesajul a fost trasmis cu succes.\n");
    	}
      }
      else if (strncmp(msg, "send", 4) == 0)
      {
        // creez primul nickname
        for (k = 4; msg[k] == ' '; ++k);
        for (j = 0; 'a' <= msg[k] && msg[k] <= 'z'; ++k, ++j)
    	   vec1[j] = msg[k];
    	vec1[j] = 0;
    	
    	// creez al doilea nickname
    	for (; msg[k] == ' '; ++k);
    	for (j = 0; 'a' <= msg[k] && msg[k] <= 'z'; ++k, ++j)
    	    vec2[j] = msg[k];
    	vec2[j] = 0;
    	
    	// creez mesajul
    	for (; msg[k] == ' '; ++k);
    	for (j = 0; 'a' <= msg[k] && msg[k] <= 'z'; ++k, ++j)
    	    vec3[j] = msg[k];
    	vec3[j] = 0;
    	
    	// caut primul tip
        for (i = 0; i < count && strcmp(vec1, V[i].nickname) != 0; ++i);
        if (i == count)
        {
    	    // nu am gasit primul tip
    	    bzero(msgrasp, 100);
    	    strcpy(msgrasp, "Could not find source name\n");
      
    	    /* returnam mesajul clientului */
    	    if (sendto(sd, msgrasp, 100, 0, (struct sockaddr*) &client, length) <= 0)
	    {
		perror ("[server]Eroare la sendto() catre client.\n");
		continue;		/* continuam sa ascultam */
	    }
    	    else
		printf ("[server]Mesajul a fost trasmis cu succes.\n");
	
	    continue;
	}
	
	// caut al doilea tip
        for (j = 0; j < count && strcmp(vec2, V[j].nickname) != 0; ++j);
        if (j == count)
        {
    	    // nu am gasit al doilea tip
    	    bzero(msgrasp, 100);
    	    strcpy(msgrasp, "Could not find destination name\n");
      
    	    /* returnam mesajul clientului */
    	    if (sendto(sd, msgrasp, 100, 0, (struct sockaddr*) &client, length) <= 0)
	    {
		perror ("[server]Eroare la sendto() catre client.\n");
		continue;		/* continuam sa ascultam */
	    }
    	    else
		printf ("[server]Mesajul a fost trasmis cu succes.\n");
	
	    continue;
	}
	
	// am mesajul
	// l-am gasit
    	bzero(msgrasp, 100);
    	strcpy(msgrasp, V[j].nickname);
    	strcat(msgrasp, ", you received from ");
    	strcat(msgrasp, V[i].nickname);
    	strcat(msgrasp, " the message ");
    	strcat(msgrasp, vec3);
    	strcat(msgrasp, "\n");

	/* returnam mesajul clientului */
    	if (sendto(sd, msgrasp, 100, 0, (struct sockaddr*) &V[j].addr, sizeof(V[j].addr)) <= 0)
	{
	  perror ("[server]Eroare la sendto() catre client.\n");
	  continue;		/* continuam sa ascultam */
	}
    	else
	    printf ("[server]Mesajul a fost trasmis cu succes.\n");
      }
      else if (strncmp(msg, "sendall", 7) == 0)
      {
        // creez primul nickname
        for (k = 4; msg[k] == ' '; ++k);
        for (j = 0; 'a' <= msg[k] && msg[k] <= 'z'; ++k, ++j)
    	   vec1[j] = msg[k];
    	vec1[j] = 0;
    	
    	// creez mesajul
    	for (; msg[k] == ' '; ++k);
    	for (j = 0; 'a' <= msg[k] && msg[k] <= 'z'; ++k, ++j)
    	    vec3[j] = msg[k];
    	vec3[j] = 0;
    	
    	// iterez tipii
        for (i = 0; i < count; ++i)
    	    if (strcmp(V[i].nickname, " ") != 0)
    	    {
    		bzero(msgrasp, 100);
    		strcpy(msgrasp, V[i].nickname);
    		strcat(msgrasp, ", you received from ");
    		strcat(msgrasp, vec1);
    		strcat(msgrasp, " the message ");
    		strcat(msgrasp, vec3);
    		strcat(msgrasp, "\n");
      
    		/* returnam mesajul clientului */
    		if (sendto(sd, msgrasp, 100, 0, (struct sockaddr*) &V[i].addr, sizeof(V[i].addr)) <= 0)
		{
		    perror ("[server]Eroare la sendto() catre client.\n");
		    continue;		/* continuam sa ascultam */
		}
    		else
		    printf ("[server]Mesajul a fost trasmis cu succes.\n");
	    }
      }
      else if (strncmp(msg, "who", 6) == 0)
      {
        // actiune de login
        for (k = 6; msg[k] == ' '; ++k);
        for (j = 0; 'a' <= msg[k] && msg[k] <= 'z'; ++k, ++j)
    	   vec1[j] = msg[k];
    	vec1[j] = 0;
        
        for (i = 0; i < count && strcmp(vec1, V[i].nickname) != 0; ++i);
        if (i == count)
        {
    	    // nu l-am gasit
    	    bzero(msgrasp, 100);
    	    strcpy(msgrasp, "You are unknown ");
    	    strcat(msgrasp, V[i].nickname);
    	    strcat(msgrasp, "\n");
      
    	    /* returnam mesajul clientului */
    	    if (sendto(sd, msgrasp, 100, 0, (struct sockaddr*) &client, length) <= 0)
	    {
		perror ("[server]Eroare la sendto() catre client.\n");
		continue;		/* continuam sa ascultam */
	    }
    	    else
		printf ("[server]Mesajul a fost trasmis cu succes.\n");
    	}
    	else
    	{
    	    strcpy(V[i].nickname, " ");
    	}
      }
      else if (strncmp(msg, "who", 3) == 0)
      {
        // actiune de login
        for (k = 6; msg[k] == ' '; ++k);
        for (j = 0; 'a' <= msg[k] && msg[k] <= 'z'; ++k, ++j)
    	   vec1[j] = msg[k];
    	vec1[j] = 0;
        
        for (i = 0; i < count && strcmp(vec1, V[i].nickname) != 0; ++i);
	bzero(msgrasp, 100);
	if (i == count)
	    strcpy(msgrasp, "NO");
	else
	    strcpy(msgrasp, "YES");
	    
	strcpy(msgrasp, "\n");
      
	/* returnam mesajul clientului */
    	if (sendto(sd, msgrasp, 100, 0, (struct sockaddr*) &client, length) <= 0)
	{
	    perror ("[server]Eroare la sendto() catre client.\n");
	    continue;		/* continuam sa ascultam */
	}
    	else
	    printf ("[server]Mesajul a fost trasmis cu succes.\n");
      }
	
      printf ("[server]Mesajul a fost receptionat...%s\n", msg);
   }
   
   return 0;
}				/* main */
