#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
  int p;
  printf("Program principal\n");

  if ((p = fork()) == -1)
  {
    printf("Eroare\n");
    return 0;
  }
  else if (p == 0)
    printf("Proces fiu, cu id %d\n", getpid());
  else
  {
    sleep(1);
    printf("Proces tata, cu id %d\n", getpid());
  }

  printf("Am terminat procesul %d\n", getpid());

  return 0;
}