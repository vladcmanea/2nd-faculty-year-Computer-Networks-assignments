/* p1.c   
   Programul ilustreaza utilizarea primitivei socketpair(); se permite 
   trimiterea de mesaje in ambele directii.
            
   Autor: Lenuta Alboaie <adria@infoiasi.ro> (c)2009  
   Modificari: VM
  */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MSG1 "Comunicam prin socketi!" 
#define MSG2 "Atentie!Socketpair() o generalizarea a pipe-urilor"

int main() 
  { 
    int sockp[2], child, kid; 
    char msg[1024], S[1024];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockp) < 0) 
      { 
        perror("Err... socketpair"); 
        exit(1); 
      }

    if ((child = fork()) == -1) perror("Err...fork"); 
    else 
      if (child)   //parinte 
        {  
            while (gets(S))
            {
    		if (write(sockp[1], S, sizeof(S)) < 0)
    		    perror("[parinte]Err...write");
        	    
    		if (read(sockp[1], S, 1024) < 0)
    		    perror("[parinte]Err...read");
    		    
    		printf("%s\n", S);
            }
            close(sockp[1]); 
            wait(NULL);
          } 
        else     //copil
          { 
            while (1)
            {
        	if (read(sockp[0], S, 1024) < 0)
        	    perror("[copil]Err...read");
        	    
        	// aici execut comanda, creez un copil caruia ii dau un exec
        	
        	if ((kid = fork()) == -1)
        	    perror("Err...fork");
        	else if (kid)
        	{
        	    wait(NULL);
        	}
        	else
        	{
        	    close(1);
        	    if (dup(sockp[0]) != 1)
        		perror("[copil]Err...dup");
        	
        	    // sunt in copil, fac execlp
        	    if (strcmp(S, "ls") == 0)
        		execlp("ls", "ls", NULL);
    		    else if (strcmp(S, "quit") == 0)
    			execlp("logout", "logout", NULL);
    		    else if (S[0] == 'p' && S[1] == 's')
    			execlp("ps", "ps", NULL);
    		    else
    			write(sockp[0], "Eroare", 6);
        	}
            }
           } 
        }             