
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find (char *path, char *tofind, char *location)
{
    DIR *dp;
    struct dirent *dirp;
    
    if ((dp = opendir(path)) == NULL)
    {
      printf ("Nu se poate deschide %s \n",path);
      exit (2);
    }
    
     while ((dirp = readdir (dp)) != NULL)
    {
      if (strcmp(dirp->d_name, tofind)==0)
      {
        strcpy(location, path);
        return 1; 
      }
      if (dirp->d_type == DT_DIR && 
          strcmp(dirp->d_name,".") != 0 &&
          strcmp(dirp->d_name,"..") != 0)
      {
        int result;
        char * newpath = calloc (PATH_MAX,sizeof(char));
        strcpy(newpath,path);
        strcat(newpath,"/");
        strcat(newpath,dirp->d_name); 
        result = find(newpath, tofind, location);
        free(newpath);
        if (result)
        {
    	    closedir(dp);
    	    return 1;
        }
      }
     }
    
    closedir(dp);
    
    return 0;
}

int
main (int argc, char *argv[])
{

  DIR *dp; 
  struct dirent *dirp;
  pid_t pid;
  char * params[2]; //valabil pentru o comanda (sau program extern) simpla fara parametri
  
  char * location = calloc(PATH_MAX, sizeof(char));
  /* verificam daca au fost furnizate argumentele */
  if (argc != 2)
    {
      fprintf (stderr, 
        "Nu ati furnizat numele programului de rulat\n");
      exit (1);
    }    
  
  find(".",argv[1],location);
  
  pid = fork();
  
  if (pid == 0)
  {
    char * toexec = calloc(PATH_MAX, sizeof(char));
    strcpy(toexec, location);
    strcat(toexec, "/");
    strcat(toexec, argv[1]);
    params[0]=malloc(30);
    strcpy(params[0], argv[1]);
    params[1]=NULL;
    execv(toexec, params);
    free(params[0]);
  }
  
  free(location);
  wait (NULL);
  exit(0);
  
}