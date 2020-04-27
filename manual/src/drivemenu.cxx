#include <stdio.h>

#include "Utility.h"

int drivemenu()
{
    int choice;
    /*PRESPLUS model;*/
      
    printf("\n1. Read PRES+ from Keyboard \n");
    printf("2. Read pres+ from a disk resident input file \n");
    printf("3. Print pres+ on screen \n");
    printf("4. Write pres+ onto a disk file \n");
    printf("5. Put the PRESPLUS model in a DOT file for visualizing\n");
    printf("6. ConstructAllPaths using cutpoints\n");
    printf("7. Quit \n");
    printf ("Enter your choice.... : ");
    scanf ("%d", &choice); 
     
    return choice ;
}
 
