#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int c1, c2, ok;
  
 
  printf("%s\n", argv[1]);
 
  int f = open(argv[1], O_RDWR);
  if (f != -1)
  {
//    for (ok = 10; ok; ok--)
//      write(f, &ok, sizeof(int));

    ok = 1;
    while (ok)
    {
      ok = 0;
      while (read(f, &c1, sizeof(int)) && read(f, &c2, sizeof(int)))
      {
        printf("%d %d\n", c1, c2);
        if (c1 > c2)
        {
          ok = 1;

          lseek(f, -2*sizeof(int), 1);
          sleep(1);
          write(f, &c2, sizeof(int));
          write(f, &c1, sizeof(int));
        }
        
        lseek(f, -sizeof(int), 1);
        
      }
      lseek(f, 0L, 0);
    }

   printf("Afisare\n");
   for (ok = 1;ok <11;ok++)
   {
      read(f,&c1,sizeof(int));
      printf("%d\n",c1);
   }

   close(f);
   
   }

  return 0;
}