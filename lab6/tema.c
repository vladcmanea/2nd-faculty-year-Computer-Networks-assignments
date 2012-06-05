#include <stdio.h>
#include <stdlib.h>

int p;
char *sir;

double factor()
{
  return (double)(sir[p++] - '0');
}

double termen()
{
  double f1, f2, rez, test;
  char sgn;

  for (rez = factor(), sgn = sir[p]; sgn == '*' || sgn == '/'; sgn = sir[p])
  {
    p++;
    if (sgn == '*')
      rez = rez * factor();
    else
    {
      test = factor();
      if (test < -0.1 || test > 0.1)
        rez = rez / test;
      else
        return 0;
    }
  }

  return rez;
}

double expresie()
{
  double t1, t2, rez;
  char sgn;

  for (rez = termen(), sgn = sir[p]; sgn == '+' || sgn == '-'; sgn = sir[p])
  {
    p++;
    if (sgn == '+')
      rez = rez + termen();
    else
      rez = rez - termen();
  }

  return rez;
}

FILE *fin, *fout;

int main()
{
  int N, p, q, s, i;
  char c;

    scanf("%d", &N);
    if (1 <= N && N <= 100)
    {
      p = fork();
      if (p == -1)
      {
        printf("Primul fork a dat eroare (main)");
        return 0;
      }
      else if (p == 0)
      {
        if (fout = fopen("in.txt", "w"))
        {
          for (i = 0; i < N; ++i)
            fprintf(fout, "%d", rand() % 10);

          fclose(fout);
        }
        else
        {
          printf("Fisierul in.txt nu a putut fi deschis");
          return 0;
        }

        return 0;
      }
      else
        sleep(1);

      p = fork();
      if (p == -1)
      {
        printf("Al doilea fork a dat eroare (main)");
        return 0;
      }
      else if (p == 0)
      {
        if (fout = fopen("op.txt", "w"))
        {
          for (i = 1; i < N; ++i)
          {
            s = rand() % 4;
            switch(s)
            {
              case 0:
                fprintf(fout, "+");
                break;
              case 1:
                fprintf(fout, "-");
                break;
              case 2:
                fprintf(fout, "*");
                break;
              case 3:
                fprintf(fout, "/");
                break;
            }
          }
          fclose(fout);
        }
        else
        {
          printf("Fisierul in.txt nu a putut fi deschis");
          return 0;
        }

        return 0;
      }
      else
        sleep(1);

      // aici sunt in parinte :)
      if (fin = fopen("in.txt", "r"))
        if (fout = fopen("op.txt", "r"))
        {
          if (sir = (char*)malloc((N << 1) * sizeof(char)))
          {
            for (i = 1, p = 0; i < N; ++i)
            {
              fscanf(fin, "%c", &c);
              sir[p++] = c;
              fscanf(fout, "%c", &c);
              sir[p++] = c;
            }
            fscanf(fin, "%c", &c);
            sir[p++] = c;
            sir[p++] = '\0';
            p = 0;
            printf("%s = ", sir);
            printf("%lf\n", expresie());
            free(sir);
          }
          else
          {
            printf("Memorie insuficienta");
            return 0;
          }

          return 0;
        }
        else
        {
          printf("Fisierul op.txt nu a putut fi deschis");
          return 0;
        }
      else
      {
        printf("Fisierul in.txt nu a putut fi deschis");
        return 0;
      }
    }
    else
    {
      printf("Numarul N este invalid (main)");
    }

  return 0;
}