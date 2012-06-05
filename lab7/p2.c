#include <stdio.h>

int main()
{
  int N, p, i;
  printf("Dati N: "); 
  scanf("%d", &N);
  if (1 <= N && N <= 10)
  {
    printf("Acesta este parintele %d\n", getpid());
    for (i = 1; i <= N; ++i)
    {
      p = fork();
      if (p == -1)
      {
        printf("Eroare\n");
        return 0;
      }
      else if (p == 0)
      {
        printf("Acesta este un fiu cu indicele %d si id %d si parinte %d\n", i, getpid(), getppid());
        return 0;
      }
    }
  }
  else
    printf("Date de intrare aiurea\n");

  return 0;
}
