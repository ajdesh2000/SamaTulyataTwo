#ifndef PRINT_MODEL
#define PRINT_MODEL
#include <stdio.h>
#include <stdlib.h>

#include "Utility.h"
 void  DecompilingEXPRNode(EXPR expr)
{

  EXPRNODE tmp = *expr ;
  if (tmp.type == 0)
  {
    switch(tmp.value)
    {
      case 1: printf("&");
      break;
      case 2: printf(">");
      break;
      case 3: printf("<");
      break;
      case 4: printf("|");
      break;
      case 5: printf(">=");
      break;
      case 6: printf("<=");
      break;
      case 7: printf("=");
      break;
      case 8: printf("!=");
      break;
      case 9: printf("+");
      break;
      case 10: printf("-");	
      break;
      case 11: printf("*"); 
      break;
      case 12: printf("/"); 
      break;
      case 13: printf("_");
      break;
     }
   }
   else if(tmp.type ==1)
   {
     printf(" (%c )",tmp.value);
   }
   else if(tmp.type == 2)
  {   
     printf(" (%d) ",tmp.value);
  }
  return ;
}  
	
void DecompilingExpr(EXPR expr)
{


  
  if (expr->left != NULL)
  {
    DecompilingExpr(expr->left);
    //printf("%s",expr->left->value);
  }

  DecompilingEXPRNode(expr);

  if(expr->right!= NULL)
  {
    DecompilingExpr(expr->right);
    //printf("%s",expr->right->value);
  }
  return ;
} 



int printpresplus(PRESPLUS model)
{ 

 char *sym_value;
        sym_value = (char * ) malloc( 100*sizeof( char ) );
  int i,j,k;

  printf("\n Number of Places = %d", model.no_of_places);  
  printf("\n Places ... \n");  
  for(i=0;i< model.no_of_places;i++) 
  {
    printf("\n Place : %s ",model.places[i].name);
    printf("\n Associated variable : %s",model.var_table[model.places[i].var_index].name);
    printf("\n Associated token value:%d",model.places[i].token_present);
    printf("\n\n") ;
  }

  printf("\n Number of Transitions = %d", model.no_of_transitions);
  printf("\n Transitions ... \n");
  for(i=0;i<model.no_of_transitions;i++)
  {
    printf("\n Transition : %s",model.transitions[i].name);
    printf("\n Guard conditions:");
    DecompilingExpr(model.transitions[i].guard);
    printf("\n Z3 Guard conditions:###");
    printf("%s", Z3_ast_to_string(model.ctx,model.transitions[i].condition));
    printf("###\n Number of preset edges : %d",model.transitions[i].no_of_preset) ;
    printf("\n Preset edge list : ") ;
    for (j=0; j < model.transitions[i].no_of_preset; j++)
    {
         printf("%s ", model.edges[model.transitions[i].preset[j]].name);
    }
    printf("\n Number of postset edges : %d",model.transitions[i].no_of_postset) ;
    printf("\n Postset edge list : ") ;
    for (j=0 ; j< model.transitions[i].no_of_postset;j++)
    {
         printf("%s ", model.edges[model.transitions[i].postset[j]].name);
    }
    printf("\n\n") ;
  } 
  
  printf("\n Number of Edges : %d",model.no_of_edges) ;
  printf("\n Edges ... : ") ;
  for(i=0;i<model.no_of_edges;i++)    
  {
    printf("\n\n Edge : %s ",model.edges[i].name);
    printf("\n Type : %d",model.edges[i].from_transition);
    if(model.edges[i].from_transition==1)
    {
      printf("\n Connects Transition %s to Place %s",model.transitions[model.edges[i].transition].name,model.places[model.edges[i].place].name);
      printf("\n Transition function :");
      DecompilingExpr(model.edges[i].expr);
      printf("\n Z3 Transition function :###");
      k=0;
      while(model.edges[i].action[k].rhs!=NULL)
      {
         	symbol_for_index( model.edges[i].action[k].lhs, sym_value );

		printf("%s  :=  ", model.var_table[model.edges[i].action[k].lhs].name/*sym_value*/ );
    printf("%s", Z3_ast_to_string(model.ctx,model.edges[i].action[k].rhs));		
		k++;
      }
      printf("###");

    }
    else{
      printf("\n Connects Place %s to Transition %s",model.places[model.edges[i].place].name,model.transitions[model.edges[i].transition].name);
    }

  }

  printf("\n\n\n Printing initial marking ... ");
  printf("\n Total number of initially marked places : %d",model.no_of_places_initially_marked);
  printf("\n Initially marked places are : ");
  for(i=0; i< model.no_of_places_initially_marked; i++)
  {
     printf("%s ",(model.places[model.initial_marking[i]].name));
  }
  printf("\n\n") ;
/*************************************/

	printf("\n Preset & postset edge list for places: \n") ;
	for(i=0;i<model.no_of_places;i++)
	{
		printf(" Preset list for place %s:\t",model.places[i].name);
		for(j=0;j<model.places[i].no_of_preset;j++)
			printf("%s ",model.edges[model.places[i].preset[j]].name);
		printf("\n");
		printf(" Postset list for place %s:\t",model.places[i].name);
		for(j=0;j<model.places[i].no_of_postset;j++)
			printf("%s ",model.edges[model.places[i].postset[j]].name);
		printf("\n");
	}


/*************************************/
return 0;
}
#endif
