/********************************************************

    Exemplu comunicare prin pipe-uri interne
    (simulare load - balancing pentru distribuirea
     calculului unei sume)
     
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char * argv[])
{
    int i;
    
    // descriptorii de pipe
    int pipeps[2];
    int pipepr[2];
    int pipec12[2];
    int pipec23[2];
    int pipec31[2];
    
    // crearea pipe-urilor
    pipe(pipeps);
    pipe(pipepr);
    pipe(pipec12);
    pipe(pipec23);
    pipe(pipec31);
    
    // capetele intervalului intre care calculam suma
    int iini = atoi(argv[1]);
    int ifin = atoi(argv[2]);
    
    // tokenul in care "strangem" rezultatul final
    int token = 0;
    
    for (i=0; i<3; i++)
    {
	// creare procese copil
	pid_t pid = fork();
	
	if (pid < 0)
	{
	    printf ("failure\n");
	}
	else
	if (pid == 0)
	{
	    switch(i)
	    {
		// primul proces copil
		case 0 : {
			    // stabilim limitele subintervalului
			    int i11 = iini;
			    int i12 = ifin/3;
			    int sum = 0;
			    int j;
			    
			    // inchidem capetele neutilizate ale pipeurilor
			    close(pipeps[1]);
			    close(pipepr[0]);
			    close(pipec12[0]);
			    close(pipec31[1]);
			    
			    //TODO: adaugarea inchiderii capetelor si pentru restul pipeurilor
			    
			    // calcul subsuma 
			    for (j = i11; j <= i12; j++)
				sum+=j;
				
			    // citire token de la parinte
			    read (pipeps[0], &token, sizeof(int));
			    
			    // actualizare token
			    token += sum;
			    
			    printf ("%d token in child 1\n", token);
			    
			    // trimitere token la urmatorul copil
			    write(pipec12[1], &token, sizeof(int));
			    
			    // citire token de la copilul al treilea
			    read (pipec31[0], &token, sizeof(int));
			    
			    // trimitere token cu rezultatul final la parinte
			    write(pipepr[1], &token, sizeof(int));
			    
			    // job done
			    exit(0);
			 }
		// al doilea proces copil - similar cu primul fara ultima parte de comunicatie
		case 1 : {
			    int i21 = ifin/3 + 1;
			    int i22 = ifin/3 * 2;
			    int sum = 0;
			    int j;
			    close(pipec23[0]);
			    close(pipec12[1]);
			    
			    for (j = i21; j <= i22; j++)
				sum+=j;
			    
			    read (pipec12[0], &token, sizeof(int));
			    token += sum;
			    
			    printf ("%d token in child 2\n", token);
			    
			    write(pipec23[1], &token, sizeof(int)); 	
			    exit(0);
			 }
		// al treilea proces copil - similar cu primul fara ultima parte de comunicatie
		case 2 : {
			    int i31 = ifin/3 * 2 + 1;
			    int i32 = ifin;
			    int sum = 0;
			    int j;
			    close(pipec31[0]);
			    close(pipec23[1]);
			    
			    for (j = i31; j <= i32; j++)
				sum+=j;
			    
			    read (pipec23[0], &token, sizeof(int));
			    token += sum;
			    
			    printf ("%d token in child 3\n", token);
			    
			    write(pipec31[1], &token, sizeof(int));
			    exit(0);
			 }	
	    }
	}
    }
    
    // inchidem pipe-urile neutilizate in parinte
    close(pipeps[0]);
    close(pipepr[1]);
    
    // TODO: inchiderea restului pipe-urilor neutilizate
    
    // trimitere token initial la primul copil - start la comunicare
    write(pipeps[1], &token, sizeof(int));
    
    // citire token cu rezultatul final de la ultimul copil
    read(pipepr[0], &token, sizeof(int));
    
    // asteptam finalizarea copiilor
    for (i=0; i<3; i++)
    {
	int status;
	wait(&status);
    }
    
    // afisam suma
    printf ("%d sum\n", token);
    
    exit(0);
}
