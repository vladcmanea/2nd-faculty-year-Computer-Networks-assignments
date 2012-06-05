/* Solutie propusa de Bogdan Alexandru Luca - exemplifica utilizarea functiei "system" pentru executia comenzilor */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define TATA 0
#define FIU  1

#define LOGIN 1
#define LS    2
#define CD    3
#define QUIT  4

#define SUCCESS 1
#define FAILED  0

int login(char *username)
{
    char buffer[100];
    FILE *file;
    
    sprintf(buffer, "cat /etc/passwd | cut -f1 -d: | grep %s >a", username );
    system(buffer);
    
    file = fopen("a", "r"); 
    fscanf(file, "%s", buffer);
    
    if(!strcmp(buffer, username))
    {
	return SUCCESS;
    }
    
    return FAILED;
}
int main() 
  { 
    int sockp[2], child, mesaj, logat = 0, lung_arg;
    char msg[1024], comanda[50], argumente[50], linie[50];
    int is_over = 0;
    
    if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockp)<0)
    {
	perror("Err...... socketpair");
	return 0;
    }
    if ((child = fork()) == -1) perror("Err...fork"); 
    else 
      if (child)   //parinte 
        {  
    	    close(sockp[TATA]);
    	    while(is_over != -1)
    	    {
    	    printf("Dati comanda:\n");
            scanf("%s", comanda);
	    
            if(!strcmp(comanda,"login:"))
            {
        	if(!logat)
        	{
        	    scanf("%s", argumente);
        	
    		    mesaj = LOGIN;
        	    write(sockp[FIU], &mesaj, 4);
        	    lung_arg = sizeof(argumente);
        	    write(sockp[FIU], &lung_arg, 4);        	
        	    write(sockp[FIU], argumente, sizeof(argumente));
        	
        	    read(sockp[FIU], &mesaj, 4);

        	    if(mesaj == FAILED)
        	    {
        		printf("Logare nereusita!\n");
        		logat = 0;
        	    }
        	    else
        	    {
        		printf("Logare reusita!\n");
        		logat = 1;
        	    }
        	}
        	else
        	{
    		    printf("Sunteti deja logat\n");	    
        	}
    	    }
            if(!strcmp(comanda, "ls"))
            {
        	if(logat)
        	{
        	    mesaj = LS;
        	    write(sockp[FIU], &mesaj, 4);
        	}
        	else
        	{
        	    printf("Nu sunteti inca logat\n");
        	}
            } 
            if(!strcmp(comanda, "cd"))
            {
        	if(logat)
        	{
        	    scanf("%s", argumente);
        	
        	    mesaj = CD;
        	    write(sockp[FIU], &mesaj, 4);
        	    lung_arg = sizeof(argumente);
        	    write(sockp[FIU], &lung_arg, 4);        	
        	    write(sockp[FIU], argumente, sizeof(argumente));
        	}
        	else
        	{
        	    printf("Nu sunteti inca logat\n");
        	}
            }
            if(!strcmp(comanda, "quit"))
            {
        	mesaj = QUIT;
        	write(sockp[FIU], &mesaj, 4);
        	read(sockp[FIU], &mesaj, 4);
        	if(mesaj == -1)
        	{
        	    printf("GATA\n");
        	    return 0;
        	}
            }
            }
            close(sockp[FIU]);
          } 
        else     
          { 
            close(sockp[FIU]); 
            while(1)
            {
    		read(sockp[TATA], &mesaj, 4);
		switch(mesaj)
		{
		    case LOGIN:
			    read(sockp[TATA], &lung_arg, 4);
			    read(sockp[TATA], argumente, lung_arg);
			    mesaj = login(argumente);
			    write(sockp[TATA], &mesaj, 4);
			    break;
		    case LS:
			    system("ls");
			    break;	
		    case CD:
			    read(sockp[TATA], &lung_arg, 4);
			    read(sockp[TATA], &argumente, lung_arg);
			    sprintf(comanda, "cd %s",argumente);
			    system(comanda);
			    break;	
		    case QUIT:
			    mesaj = -1;
			    write(sockp[TATA], &mesaj, 4);
			    return 0;	
		}
	    }
            close(sockp[TATA]);
           } 
        }             