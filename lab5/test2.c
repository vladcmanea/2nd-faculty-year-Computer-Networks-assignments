#include <stdio.h>
#include <fcntl.h>

int main(int arg, char* argv[])
{
  int fin, fout;
  char c, d, e;
  int count = 0, max = 0;

  fin = open(argv[1], O_RDONLY);
  //fout = open(argv[2], O_WRONLY);

  if (fin == -1) fin = 0;
  if (fout == -1) fout = 1;
  
  if (arg == 0)
    e = ' ';
  else
    e = argv[3][1];

  if (e == 'c')
  {
    while (read(fin, &c, 1) != 0) 
      ++count;

    printf("%d\n", count);
  }
  if (e == 'w')
  {
    for (d = '#', count = 0; read(fin, &c, 1) != 0; d = c)
      if (!('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z') && ('a' <= d && d <= 'z' || 'A' <= d && d <= 'Z'))
        count++;

    if ('a' <= d && d <= 'z' || 'A' <= d && d <= 'Z')
      count++;

    printf("%d\n", count);
  }
  if (e == 'l')
  {
    for (count = 0; read(fin, &c, 1) != 0; count += (c == 10));
    if (count > 0) count++;

    printf("%d\n", count);

    return 0;
  }
  if (e == 'L')
  {
    for (count = 0; read(fin, &c, 1) != 0;)
    {
      count++;
      if (c == 10)
      {
         max = max < count-1? count-1: max;
         count = 0;
      }
    }

    if (count > max)
      max = count;

    printf("%d\n", max);
  }

  close(fin);

  return 0;
}
