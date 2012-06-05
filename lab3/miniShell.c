/* Solutie propusa de Andrei Mihai Livadariu - exemplifica utilizarea "grep" prin pipe pentru "login" */

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define STATUS_SUCCESS 0
#define STATUS_FAIL 1
#define STATUS_EXIT 2
#define STATUS_CD 3

#define MAX_COMMAND_LENGTH 50
#define MAX_COMMAND_ARGS 10

// Daca e vreo problema in copil se paseaza executia aici
int workerException( int sock )
{
   char sendStatus = STATUS_EXIT;
   write( sock, &sendStatus, sizeof( int ) );
   return 1;
}

// Procesul copil
int workerThread( int sock )
{
   int sendStatus;
   int execStatus;
   int recLen;
   int directoryLen;
   int i;
   int loginPipe[2];
   int loginNameLength;
   char loginFile[] = "/etc/passwd";
   char directoryName[ PATH_MAX ];
   char command[ MAX_COMMAND_LENGTH ];
   char *passArgs[ MAX_COMMAND_ARGS ];
   char *token;
   
   do
     {	
	// Asteptam sa citim comanda transmisa de parinte
	while( read( sock, &recLen, sizeof( int ) ) != sizeof( int ) )
	  {}
	if( read( sock, command, recLen ) != recLen )
	  {
	     fprintf( stderr, "Eroare la read in copil!\n" );
	     return workerException( sock );
	  }
	
	// Parsam comanda si separam argumentele
	i = 0;
	passArgs[0] = strtok( command, " " );
	while( ( token = strtok( NULL, " " ) ) != NULL )
	  {
	     passArgs[ ++i ] = token;
	  }
	passArgs[ ++i ] = NULL;	     
	
	// Cazuri pentru fiecare din comenzi
	if( strcmp( passArgs[0], "test" ) == 0 )
	  {
	     sendStatus = STATUS_SUCCESS;
	     printf( "Comanda test executata!\n" );
	  }
	else if( strcmp( passArgs[0], "exit" ) == 0 )
	  {
	     sendStatus = STATUS_EXIT;
	     printf( "Exiting...\n" );
	  }
	else if( strcmp( passArgs[0], "cd" ) == 0 )
	  {
	     if( chdir( passArgs[1] ) == 0 )
	       {
		  sendStatus = STATUS_CD;
	       }
	     else
	       {
		  sendStatus = STATUS_FAIL;
	       }
	  }
	else if( strcmp( passArgs[0], "login" ) == 0 )
	  {
	     switch( fork() )
	       {
		case -1:
		  sendStatus = STATUS_FAIL;
		  break;
		case 0:
		  strcpy( passArgs[0], "grep" );
		  loginNameLength = strlen( passArgs[1] );
		  passArgs[1][ loginNameLength ] = ':';
		  passArgs[1][ loginNameLength + 1 ] = '\0';
		  passArgs[1] = passArgs[1] - 1;
		  passArgs[1][0] = '^';
		  passArgs[2] = loginFile;
		  passArgs[3] = NULL;
		  
		  if( pipe( loginPipe ) == 1 )
		    {
		       sendStatus = STATUS_FAIL;
		       break;
		    }
		  close( loginPipe[0] );
		  dup2( loginPipe[1], 1 );
		  
		  execv( "/bin/grep", passArgs );
		  return STATUS_FAIL;
		default:
		  wait( &execStatus );
		  if( WEXITSTATUS( execStatus ) == 0 )
		    {
		       sendStatus = STATUS_SUCCESS;
		       printf( "Login cu succes.\n" );
		    }
		  else
		    {
		       sendStatus = STATUS_FAIL;
		       printf( "Nume necunoscut.\n" );
		    }
	       }
	  }
	// Pentru celelalte comenzi, se executa direct
	else
	  {
	     switch( fork() )
	       {
		case -1:
		  sendStatus = STATUS_FAIL;
		  break;
		case 0:
		  execvp( passArgs[0], passArgs );
		  return STATUS_FAIL;
		default:
		  wait( &execStatus );
		  sendStatus = WEXITSTATUS( execStatus ) == 0 ? STATUS_SUCCESS : STATUS_FAIL;
	       }   
	  }
	fflush( stdout );
	
	// Transmitem rezultatul executiei
	if( write( sock, &sendStatus, sizeof( int ) ) != sizeof( int ) )
	  {
	     fprintf( stderr, "Eroare la write in copil!\n" );
	     return workerException( sock );
	  }
	
	// Daca am avut comanda cd, trebuie transmis noul director
	if( sendStatus == STATUS_CD )
	  {
	     getcwd( directoryName, PATH_MAX );
	     directoryLen = strlen( directoryName ) + 1;
	     if( write( sock, &directoryLen, sizeof( int ) ) != sizeof( int ) )
	       {
		  fprintf( stderr, "Eroare la write in copil!\n" );
		  return workerException( sock );
	       }
	     write( sock, &directoryName, directoryLen );
	  }
	// Daca am avut exit, iesim
	else if( sendStatus == STATUS_EXIT )
	  {
	     return 0;
	  }
     }while( 1 );
   return 0;
}

//Procesul parinte
int main( int argc, char* argv[] )
{
   int sockp[2], child;
   int sendLen;
   int recStatus = STATUS_SUCCESS;
   int recDirLength;
   int commandLength;
   char command[ MAX_COMMAND_LENGTH ];
   char directoryName[ PATH_MAX ];
   
   // Deschidem socketul
   if( socketpair( AF_UNIX, SOCK_STREAM, 0, sockp ) == -1 )
     {
	fprintf( stderr, "Eroare la socketpair!\n" );
	return 2;
     }
   
   // Creem copilul
   switch( child = fork() )
     {
      case -1:
	fprintf( stderr, "Eroare la fork!\n" );
	return 1;
      case 0:
	close( sockp[1] );
	return workerThread( sockp[0] );
     }
   
   // Inchidem capatul corespunzator copilului
   close( sockp[0] );
   
   // Directorul din care pornim
   getcwd( directoryName, PATH_MAX );
   
   do
     {
	// Asteptam comanda
	printf( "%s > ", directoryName );
	fgets( command, MAX_COMMAND_LENGTH, stdin );
	
	// Inlocuim \n de la urma cu \0
	sendLen = strlen( command );
	command[ sendLen - 1 ] = '\0';
	
	// Transmitem lungimea comenzii
	if( write( sockp[1], &sendLen, sizeof( int ) ) != sizeof( int ) )
	  {
	     fprintf( stderr, "Eroare la write in parinte!\n" );
	     return 3;
	  }
	
	// Transmitem comanda efectiva
	if( write( sockp[1], command, sendLen ) != sendLen )
	  {
	     fprintf( stderr, "Eroare la write in parinte!\n" );
	     return 3;
	  }
	
	// Asteptam un raspuns
	while( read( sockp[1], &recStatus, sizeof( int ) ) != sizeof( int ) )
	  {}
	
	// Daca e cazul sa schimbam directorul mai trebuie sa citim noua cale
	if( recStatus == STATUS_CD )
	  {
	     while( read( sockp[1], &recDirLength, sizeof( int ) ) != sizeof( int ) )
	       {}
	     read( sockp[1], directoryName, recDirLength );
	  }
	// Daca e cazul sa iesim trebuie sa facem wait
	else if( recStatus == STATUS_EXIT )
	  {
	     wait( NULL );
	     return 0;
	  }
	fflush( stdout );
     }while( 1 );
   return 0;
}
