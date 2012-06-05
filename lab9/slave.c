#include <stdio.h>

int x, sum, aux;

FILE *fin;

int main(int argc, char *argv[])
{
    if (fin = fopen(argv[1], "r"))
    {
	while(!feof(fin))
	{
	    fscanf(fin, "%d", &x);
	    sum += x;
	    aux = x;
	}

	fclose(fin);
    }
    else
    {
	printf("Eroare la citirea fisierului\n");
	return 0;
    }
    
    if (fin = fopen(argv[2], "w"))
    {
	fprintf(fin, "%d ", sum-aux);
	fclose(fin);
    }
    else
    {
	printf("Eroare la scrierea fisierului\n");
	return 0;
    }
    
    return 0;
}
