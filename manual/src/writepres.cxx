#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utility.h"
 void  DecompilingEXPRNodefile(FILE *fp, EXPR expr)
{

 
  
  EXPRNODE tmp = *expr ;
  if (tmp.type == 0)
  {
    switch(tmp.value)
    {
      case 1: fprintf(fp,"&");
      break;
      case 2: fprintf(fp,">");
      break;
      case 3: fprintf(fp,"<");
      break;
      case 4: fprintf(fp,"|");
      break;
      case 5: fprintf(fp,">=");
      break;
      case 6: fprintf(fp,"<=");
      break;
      case 7: fprintf(fp,"=");
      break;
      case 8: fprintf(fp,"!=");
      break;
      case 9: fprintf(fp,"+");
      break;
      case 10: fprintf(fp,"-");	
      break;
      case 11: fprintf(fp,"*"); 
      break;
      case 12: fprintf(fp,"/"); 
      break;
      case 13: fprintf(fp,"_");
      break; 
    }
   }
   else if(tmp.type ==1)
   {
     fprintf(fp,"%c",tmp.value);
   }
   else if(tmp.type == 2)
  {   
     fprintf(fp,"%d",tmp.value);
  }
  return ;
}  
	
void DecompilingExprfile(FILE *fp,EXPR expr)
{
  

  if (expr->left != NULL)
  {
    DecompilingExprfile(fp, expr->left);
  }

  DecompilingEXPRNodefile(fp,expr);

  if(expr->right!= NULL)
  {
    DecompilingExprfile(fp, expr->right);
  }
  return ;
} 	 





	 
int writeFile (PRESPLUS model)
{ 
 FILE *f;
  char *sym_value;
        sym_value = (char * ) malloc( 100*sizeof( char ) );
 char name[100];
 printf(".....Enter file name.......... :\n");
 scanf("%s", name);
 //char prename[200]="./examples/presplus/";
 //strcat(prename,name);
 //fp = fopen(prename,"w");
 f = fopen(name, "w");
 int i,j,k;
 // VARIABLE temp1;
  //ffprintf(fp,"**********Printing PRES+ on file******************** ");






fprintf(f,"\n Number of Places = %d", model.no_of_places);  
  fprintf(f,"\n Places ... \n");  
  for(i=0;i< model.no_of_places;i++) 
  {
    fprintf(f,"\n Place : %s ",model.places[i].name);
    fprintf(f,"\n Associated variable : %s",model.var_table[model.places[i].var_index].name);
    fprintf(f,"\n Associated token value:%d",model.places[i].token_present);
    fprintf(f,"\n\n") ;
  }

  fprintf(f,"\n Number of Transitions = %d", model.no_of_transitions);
  fprintf(f,"\n Transitions ... \n");
  for(i=0;i<model.no_of_transitions;i++)
  {
    fprintf(f,"\n Transition : %s",model.transitions[i].name);
    fprintf(f,"\n Guard conditions:");
    DecompilingExprfile(f,model.transitions[i].guard);
    fprintf(f,"\n Z3 Guard conditions:###");
    fprintf(f,"%s", Z3_ast_to_string(model.ctx,model.transitions[i].condition));
    fprintf(f,"###\n Number of preset edges : %d",model.transitions[i].no_of_preset) ;
    fprintf(f,"\n Preset edge list : ") ;
    for (j=0; j < model.transitions[i].no_of_preset; j++)
    {
         fprintf(f,"%s ", model.edges[model.transitions[i].preset[j]].name);
    }
    fprintf(f,"\n Number of postset edges : %d",model.transitions[i].no_of_postset) ;
    fprintf(f,"\n Postset edge list : ") ;
    for (j=0 ; j< model.transitions[i].no_of_postset;j++)
    {
         fprintf(f,"%s ", model.edges[model.transitions[i].postset[j]].name);
    }
    fprintf(f,"\n\n") ;
  } 
  
  fprintf(f,"\n Number of Edges : %d",model.no_of_edges) ;
  fprintf(f,"\n Edges ... : ") ;
  for(i=0;i<model.no_of_edges;i++)    
  {
    fprintf(f,"\n\n Edge : %s ",model.edges[i].name);
    fprintf(f,"\n Type : %d",model.edges[i].from_transition);
    if(model.edges[i].from_transition==1)
    {
      fprintf(f,"\n Connects Transition %s to Place %s",model.transitions[model.edges[i].transition].name,model.places[model.edges[i].place].name);
      fprintf(f,"\n Transition function :");
      DecompilingExprfile(f,model.edges[i].expr);
      fprintf(f,"\n Z3 Transition function :###");
      k=0;
      while(model.edges[i].action[k].rhs!=NULL)
      {
          symbol_for_index( model.edges[i].action[k].lhs, sym_value );

    fprintf(f,"%s  :=  ", model.var_table[model.edges[i].action[k].lhs].name/*sym_value*/ );
    fprintf(f,"%s", Z3_ast_to_string(model.ctx,model.edges[i].action[k].rhs));    
    k++;
      }
      fprintf(f,"###");

    }
    else{
      fprintf(f,"\n Connects Place %s to Transition %s",model.places[model.edges[i].place].name,model.transitions[model.edges[i].transition].name);
    }

  }

  fprintf(f,"\n\n\n Printing initial marking ... ");
  fprintf(f,"\n Total number of initially marked places : %d",model.no_of_places_initially_marked);
  fprintf(f,"\n Initially marked places are : ");
  for(i=0; i< model.no_of_places_initially_marked; i++)
  {
     fprintf(f,"%s ",(model.places[model.initial_marking[i]].name));
  }
  fprintf(f,"\n\n") ;
/*************************************/

  fprintf(f,"\n Preset & postset edge list for places: \n") ;
  for(i=0;i<model.no_of_places;i++)
  {
    fprintf(f," Preset list for place %s:\t",model.places[i].name);
    for(j=0;j<model.places[i].no_of_preset;j++)
      fprintf(f,"%s ",model.edges[model.places[i].preset[j]].name);
    fprintf(f,"\n");
    fprintf(f," Postset list for place %s:\t",model.places[i].name);
    for(j=0;j<model.places[i].no_of_postset;j++)
      fprintf(f,"%s ",model.edges[model.places[i].postset[j]].name);
    fprintf(f,"\n");
  }
  fprintf(f,"\n\n") ;
  fclose(f);
  return 0;
}
