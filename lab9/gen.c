#include <stdio.h>

int N, suma, x;
FILE *fin;

int main()
{
    printf("Scrieti numarul de numere: ");
    scanf("%d", &N);
    
    if (fin = fopen("data.txt", "w"))
    {
        fprintf(fin, "%d ", N);
        while (N--)
        {
    	    x = rand() % 10;
	    fprintf(fin, "%d ", x);
	    suma += x;
	}
	
	fprintf(fin, "%d ", suma);
	fclose(fin);
    }
    else
	printf("Eroare din fisier\n");
    
    return 0;
}