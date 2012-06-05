/* Solutie propusa de Lucian Constantin Galea - exemplifica utilizarea unei functii de parsare a fisierului pentru "login" */

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int find(int fd,char *s)
{
    int i=0,j=0;
    char c;
    
    lseek(fd,0,0);
    
    do
    {

        
        j=read(fd,&c,1);
        
        if(c=='\n')
        {
    	    read(fd,&c,1);
    	    
    	    if(c==s[0])
    	    {
        	while(c==s[i])
        	{
            	    read(fd,&c,1);
            	    i++;
            	    if(i==(strlen(s)-1)&&c==':') return 1;
        	}
    	    
    	    }
        i=0;
	
	}
    }while(j!=0||j==-1);
    
    return 0;
}


int main()
{
    char comanda[100];
    char comanda2[100];
    int sck[2],pid,terminated=0;
    
    if(socketpair(AF_UNIX,SOCK_STREAM,0,sck)==-1) 
    {
	perror("Eroare la socket\n");
    }
    
    if((pid=fork())==-1) 
    {
	perror("Eroare la fork\n");
    }
    
    if(pid)
    {
	char raspuns[15];
	int x,y,z,t,v,l,w;
	char director_curent[PATH_MAX];
	while(!terminated)
	{
	
	    fgets(comanda,40,stdin);
	    
	    write(sck[0],comanda,40);
	    read(sck[0],raspuns,15);

	
	    x=strcmp(raspuns,"over");
	    y=strcmp(raspuns,"neexecutat");
	    z=strcmp(raspuns,"executat");	    
	    t=strcmp(raspuns,"schimbat");
	    v=strcmp(raspuns,"nu pot schimba");
	    l=strcmp(raspuns,"Nu cunosc");
	    w=strcmp(raspuns,"Director");
	    
	    if(!x) terminated=1;
	    if(!y) printf("Userul nu exista\n");
	    if(!z) printf("Comanda executata cu succes\n\n");
	    if(!t) printf("Directorul a fost schimbat\n");
	    if(!v) printf("Nu se poate schimba directorul\n");
	    if(!l) printf("Comanda necunoscuta\n");
	    
	    if(!w)
	    {
		
		write(sck[0],"Poti scrie",10);
		
		read(sck[0],director_curent,PATH_MAX);
		printf("Directorul curent este %s\n",director_curent);
			    
	    } 
	}
    }
    
    if(!pid)
    {
	
	
	
	const char raspuns1[15]="executat";
	const char raspuns2[15]="over";
	const char raspuns3[15]="neexecutat";
	const char raspuns4[15]="schimbat";
	const char raspuns5[15]="nu pot schimba";
	char directorul_curent[PATH_MAX];
	char *test;
	char *test2;
	char* username;
	char* cale;
	int fd=open("/etc/passwd",O_RDONLY),pid2,x,y,z,retur;
	char bla[10];
	while(!terminated)
	{
	    read(sck[1],comanda,40);
	
	    x=strcmp(comanda,"ls\n");
	    y=strcmp(comanda,"quit\n");
	    z=strcmp(comanda,"pwd\n");
	    test=strstr(comanda,"login");
	    test2=strstr(comanda,"cd");
	    
	    if(!x)
	    {
		
		if((pid2=fork())==-1) 
		{
		    perror("Eroare la fork in copil\n");
		}
		
		if(!pid2) execlp("ls","ls",NULL);
		
		else write(sck[1],raspuns1,15);
	    }
	    
	    if(!y)
	    {
		terminated=1;
		write(sck[1],raspuns2,15);
	    }
	    
	    if(!z)
	    {
		write(sck[1],"Director",15);
		
		getcwd(directorul_curent,PATH_MAX);
		
		read(sck[1],bla,10);
		
		write(sck[1],directorul_curent,PATH_MAX);	
		
		
	    }
	    
	    if(test!=NULL)
	    {
		
	        
			
		username=(char*)malloc(strlen(comanda+6));
		strcpy(username,comanda+6);
		
		retur=find(fd,username);
		if(retur==1) write(sck[1],raspuns1,15);
		if(retur==0) write(sck[1],raspuns3,15);
		free(username);
		
	    }
	    
	    if(test2!=NULL)
	    {
		cale=(char*)malloc(strlen(comanda+3));
		
		strcpy(cale,comanda+3);
		
		cale[strlen(cale)-1]='\0';
		
		if(chdir(cale)==-1) write(sck[1],raspuns5,15);
	        
	        else write(sck[1],raspuns4,15);
		
		free(cale);
	    }    
	    
	    if(x!=0&&test2==NULL&&test==NULL&&y!=0&&z!=0)
	    {
		write(sck[1],"Nu cunosc",15);
	    }
	}    
    }
    




}
