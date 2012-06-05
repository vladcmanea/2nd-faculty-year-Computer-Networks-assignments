/* myfind

TODO:
- add error checking
- add comments
*/

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
  char * location = calloc(PATH_MAX, sizeof(char));

  if (argc != 2)
    {
      fprintf (stderr, 
        "Nu ati furnizat numele fisierului cautat\n");
      exit (1);
    }    
  
  if (find(".",argv[1],location))
  {
   printf("Gasit in %s", location);
  }
  else 
   printf("Nu exista");
  
  /* am terminat */
  exit (0);
}
