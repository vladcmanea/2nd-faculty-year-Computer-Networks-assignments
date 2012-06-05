/******************************************

Varianta minimala (fara error checking)
de exemplificare a citirii unui set de
campuri incapsulate intr-un fisier.

*******************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[])
{
    // definim campurile
    // (mai elegant : intr-o structura)
    uint16_t source;
    uint16_t dest;
    uint16_t length;
    uint16_t checksum;
    char data[32];
    
    // deschidem fisierul
    // (mai elegant : dat ca argument)
    int fd = open("testpak", O_RDONLY);
    
    // citim fiecare camp din fisier
    read (fd, &source, 2);
    read (fd, &dest, 2);
    read (fd, &length, 2);
    read (fd, &checksum, 2);
    read (fd, data, 32);
    
    // afisam campurile pe ecran
    printf("source = %d \n"
	   "destination = %d \n"
	   "length = %d \n"
	   "checksum = %d \n"
	   "data = %s",
	    source,
	    dest,
	    length,
	    checksum,
	    data);
	    
    close(fd);	    

    return 0;      
}
