#include "Utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//UNCOMMENT THE REQUIRED COMMENTED LINES IN CASE TWO MODELS ARE TO BE INPUTTED
int main(int argc, char** argv) {
  int n,m;
  PRESPLUS model1,model2;
  //Initialize contexts attached to both models
    PATHSET paths1,paths2;
  // if(argc==3) {		
  //   FILE *fp1 = fopen(argv[1],"r");
  //   FILE *fp2 = fopen(argv[2],"r");
  //   model1 = readFile (fp1);
	 //  fclose(fp1);
  //   model2 = readFile (fp2);
  //         fclose(fp2);
  //    //findEquivalent(model1,model2);
  //   char* s2=argv[2],*s3=(char*)calloc(100,sizeof(char));
  //   for(n=16;s2[n]!='\0';n++)
  //     s3[n-16]=s2[n];
  //   return 0;
  // }
 
  while(n!=8) {
    n = drivemenu();
    switch (n) {




      case 1: {
        printf("*********Reading from KBD****************\n");
        model1 = readKBDF();
      }
       break;






      case 2: {                
  			model1 = readFile();
  			
     //            printf("Do u want enter another file then press 1 else press 0\n");
     //            scanf("%d",&m);
     //            if(m==1)
     //            {
     //            printf("..........Enter file name :.........\n");
    	// 	scanf("%s", name);
    	// 	fp = fopen(name,"r");                 
  			// model2 = readFile(fp);
  			// fclose(fp);
     //             }
  		}
       break;






      case 3: {
        printf("........model1..........\n");
        printpresplus(model1);
        
        // if(m==1){
        //   printf("........model2..........\n");
        //   printpresplus(model2);
        // }
       }
       break;







      case 4: {
        writeFile(model1);
       // if(m==1){
       // writeFile(model2);}
      }
       break;






      
    




      case 5: {
         visualize(model1); 
         //if(m==1)
         //visualize(model2);     
       }    
       break;








       case 6: {
        paths1=ConstructAllPaths(&model1);
        // if(m==1){
        //     paths2=ConstructAllPaths(&model2);
        //  }
      }
       break;
      default:
        return 1;
        break; 
    }  
  }
  return 0 ; 
}
