#include <stdio.h>
#include <unistd.h>

int n, sum, x, N, M, i, r, p, aux;

char sir1[100], sir2[100];

FILE *fin, *fim;

int main()
{
    if (fim = fopen("data.txt", "r"))
	fscanf(fim, "%d", &M);
    else
    {
	printf("Eroare fisier\n");
	return 0;
    }

    //printf("Dati numarul de slave: ");
    //scanf("%d", &N);
    N = 100;
    
    for (i = 1; i <= N; ++i)
    {
	sprintf(sir1, "f%d.in\0", i);
	if (fin = fopen(sir1, "w"))
	{
	    fprintf(fin, " ");
	    fclose(fin);
	}
    }
    
    for (i = 1; i <= M; ++i)
    {
	fscanf(fim, "%d", &x);
	
	sprintf(sir1, "f%d.in\0", 1 + i % N);
	if (fin = fopen(sir1, "a"))
	{
	    fprintf(fin, "%d ", x);
	    fclose(fin);
	}
	else
	{
	    printf("Eroare de fisier");
	    return 0;
	}
    }
    
    for (n = 1; n <= N; ++n)
    {
	p = fork();
	if (p == -1)
	{
	    printf("Eroare: fork n-a mers in procesul %d\n", getpid());
	    return 0;
	}
	if (p == 0)
	{
	    sprintf(sir1, "f%d.in\0", n);
	    sprintf(sir2, "f%d.out\0", n);
	    execl("slave", "slave", sir1, sir2, NULL);
	}
	else
	    x = wait (&i);
    }
    
    for (n = 1; n <= N; ++n)
    {
	sprintf(sir1, "f%d.out\0", n);
	if (fin = fopen(sir1, "r"))
	{
	    fscanf(fin, "%d", &aux);
	    sum += aux;
	    fclose(fin);
	}
	else
	{
	    printf("Eroare la deschidere\n");
	    return 0;
	}
    }
    
    printf("%d\n", sum);
    
    return 0;
}