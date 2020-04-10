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

void 
write_listsfile(FILE *fp, NC *root ){

  char *sym_value;
  sym_value = (char * ) malloc( 1000*sizeof( char ) );
  
  if( root != NULL ){
    if( root->type == 'R' || root->type == 'O' ){
      if( root->type == 'R' )
	printf( " ( ");
      write_lists( root->link );
    }
    
    switch( root->type )
      {
      case 'f':
	symbol_for_index( root->inc, sym_value );
	fprintf(fp, "* %s( ",sym_value );
	break;	
      case 'v': 
	symbol_for_index( root->inc, sym_value );
        fprintf(fp, "* %s ",sym_value );
	break;
      case 'T':
	fprintf(fp, "%c %d ",( root->inc >= 0 )?'+':'-', 
		abs( root->inc ) );
	break;
      case 'S':
	fprintf(fp, " %d ",root->inc );
	break;
      case 'R':
	switch( root->inc )
	  {
	  case 0: fprintf(fp, ">= 0");
	    break;
	  case 1: fprintf(fp, "> 0" );
	    break;
	  case 2: fprintf(fp, "<= 0" );
	    break;
	  case 3: fprintf(fp, "< 0" );
	    break;
	  case 4: fprintf(fp, "== 0");
	    break;
	  case 5: fprintf(fp, "!= 0" );
	    break;
	  }; // switch( root->inc )
	printf( " ) " );
	if( root->list != NULL )
	  fprintf(fp, " OR " );
	break;
      case 'A':  
	break;
      case 'O':
	if( root->list != NULL )
	  fprintf(fp, " AND ");
	break;
      case 'D':
	fprintf(fp, " / ");
	break;
      default: fprintf(fp, "%c %d\t",  root->type, root->inc );
      };
    if( root->type != 'R' && root->type != 'O' )	
      write_listsfile(fp, root->link );
    if( root->type == 'f' )
      fprintf(fp, " )" );
    if( root->type == 'S' && root->list != NULL )
      fprintf(fp, ", " );
    write_listsfile(fp, root->list );
  }
  return;
}	 




	 
int writeFile (PRESPLUS model)
{ 
 FILE *fp;
  char *sym_value;
        sym_value = (char * ) malloc( 100*sizeof( char ) );
 char name[100];
 printf(".....Enter file name.......... :\n");
 scanf("%s", name);
 //char prename[200]="./examples/presplus/";
 //strcat(prename,name);
 //fp = fopen(prename,"w");
 fp = fopen(name, "w");
 int i,j,k;
 // VARIABLE temp1;
  //ffprintf(fp,"**********Printing PRES+ on file******************** ");






  fprintf(fp,"\n Number of Places = %d", model.no_of_places);  
  fprintf(fp,"\n Printing Places ... \n");  
  
  for(i=0;i< model.no_of_places;i++) 
  {
    fprintf(fp,"\n Place : %s ",model.places[i].name);
    fprintf(fp,"\n Associated variable : %s",model.var_table[model.places[i].var_index].name);
    fprintf(fp,"\n Associated token value:%d",model.places[i].token_present);
    fprintf(fp,"\n\n") ;
  }

  fprintf(fp,"\n Number of Transitions = %d", model.no_of_transitions);
  fprintf(fp,"\n Printing Transitions ... \n");
  for(i=0;i<model.no_of_transitions;i++)
  {
    fprintf(fp,"\n Transition : %s",model.transitions[i].name);
    fprintf(fp,"\n Guard conditions:");
    DecompilingExprfile(fp,model.transitions[i].guard);
    fprintf(fp,"\n Normalized Guard conditions:");
    write_listsfile(fp,model.transitions[i].condition);
    fprintf(fp,"\n Printing preset edge list : ") ;
    for (j=0; j < model.transitions[i].no_of_preset; j++)
    {
         fprintf(fp,"%s ", model.edges[model.transitions[i].preset[j]].name);
    }
    printf("\n Printing postset edge list : ") ;
    for (j=0 ; j< model.transitions[i].no_of_postset;j++)
    {
         fprintf(fp,"%s ", model.edges[model.transitions[i].postset[j]].name);
    }
    fprintf(fp,"\n\n") ;
  } 
  
  fprintf(fp,"\n Number of Edges : %d",model.no_of_edges) ;
  fprintf(fp,"\n Printing Edges ... : ") ;
  for(i=0;i<model.no_of_edges;i++)    
  {
    fprintf(fp,"\n\n Edge : %s ",model.edges[i].name);
    if(model.edges[i].from_transition==1)
    {
      fprintf(fp,"\n Connects Transition %s to Place %s",model.transitions[model.edges[i].transition].name,model.places[model.edges[i].place].name);
      fprintf(fp,"\n Transition function :");
      DecompilingExprfile(fp,model.edges[i].expr);
      fprintf(fp,"\n Normalized Transition function :");
      k=0;
      while(model.edges[i].action[k].rhs!=NULL)
      {
         	symbol_for_index( model.edges[i].action[k].lhs, sym_value );
		fprintf(fp,"\n%s  :=  ", sym_value );		
		write_listsfile(fp,model.edges[i].action[k].rhs);
		k++;
      }

    }
    else{
      fprintf(fp,"\n Connects Place %s to Transition %s",model.places[model.edges[i].place].name,model.transitions[model.edges[i].transition].name);
    }

  }

  fprintf(fp,"\n\n\n Printing initial marking ... ");
  fprintf(fp,"\n Total number of initially marked places : %d",model.no_of_places_initially_marked);
  fprintf(fp,"\n Initially marked places are : ");
  for(i=0; i< model.no_of_places_initially_marked; i++)
  {
     fprintf(fp,"%s ",(model.places[model.initial_marking[i]].name));
  }
  fprintf(fp,"\n\n") ;
  fclose(fp);
  return 0;
}
