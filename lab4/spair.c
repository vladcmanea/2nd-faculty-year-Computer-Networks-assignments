
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() 
  { 
    int sockp[2], child; 
    char msg[1024];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockp) < 0) 
      { 
        perror("Err... socketpair"); 
        exit(1); 
      }

    if ((child = fork()) == -1) perror("Err...fork"); 
    else 
      if (child)   //parinte 
        {  
            close(sockp[0]); 
            printf ("Enter message: ");
            fgets (msg, 1024, stdin);
            msg[strlen(msg)-1]='\0';
            
            if (write(sockp[1], msg, sizeof(msg)) < 0) perror("[parinte]Err...write"); 
            if (read(sockp[1], msg, 1024) < 0) perror("[parinte]Err...read"); 
            
            printf ("Result received: %s",msg);
            close(sockp[1]); 
            wait(NULL);
          } 
        else     //copil
          { 
            close(sockp[1]); 
            if (read(sockp[0], msg, 1024) < 0) perror("[copil]Err..read"); 
            
            strcat (msg, " + processed");
            
            if (write(sockp[0], msg, sizeof(msg)) < 0) perror("[copil]Err...write"); 
            close(sockp[0]);
           } 
        }             