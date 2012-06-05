#include <stdio.h>

int suma, x, y;
FILE *fin;

int main()
{
    if (fin = fopen("data.txt", "r"))
    {
	fscanf(fin, "%d", &x);
	
	while (x--)
	{
	    fscanf(fin, "%d", &y);
	    suma += y;
	}
    }
    else
	printf("Eroare bla bla\n");
	
    printf("%d", suma);
	
    return 0;



}
