#include <stdio.h>
#include <sys/wait.h>

void createfork(int k, int l)
{
  int p;
  if (k == 0)
  {
    return;
  }
  else
  {
    p = fork();
    if (p == -1)
    {
      printf("Eroare\n");
      return;
    } 
    else if (p == 0)
    {
      printf("Fiu %d id %d parinte %d\n", l-k+1, getpid(), getppid());
      createfork(k-1, l);
    }
    else
      return;
  }
  
}

int main()
{
  int N;
  scanf("%d", &N);
  
  printf("parinte: %d\n", getpid());
  if (1 <= N && N <= 10)
  createfork(N, N);
  return 0;
}