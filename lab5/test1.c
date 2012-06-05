#include <stdio.h>
#include <fcntl.h>

int a[256];

int main(int argc, char* argv[])
{
    char c;
    int i;
    
    int f = open(argv[1], O_RDONLY);
    if (f != -1)
        while (read(f, &c, 1) != 0)
            a[c]++;

    for (i = 0; i < 256; ++i)
        if (a[i])
            printf("Numarul de aparitii ale caracterului %c - %d este %d\n", i,i, a[i]);

    close(f);

    return 0;
}
