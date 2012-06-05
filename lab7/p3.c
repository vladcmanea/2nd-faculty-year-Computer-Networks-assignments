#include <stdio.h>
#include <sys/types.h>

int main()
{
  int p, N;
  scanf("%d", &N);

  while (N--)
  {
    p = fork();
    if (p == -1)
    {
      printf("Eroare\n");
      return 0;
    }
    if (p == 0)
    {
      printf("Sunt in fiul %d id %d cu parent %d\n", N, getpid(), getppid());
      p = fork();
      if (p == -1)
        printf("Eroare\n");
      else if (p)
        return 0;
    }
  }
  
  return 0;
}