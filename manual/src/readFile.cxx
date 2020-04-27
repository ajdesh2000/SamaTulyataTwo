#include <stdio.h>
#include <stdlib.h>
#include "Utility.h"
SYMTAB  stab;

SYMTAB  stab1;

/*********************************************************************************/

/*   This function is used to find the index value of a symbol in a symbol table */
/*   If the symbol is already in the table then it will return the index  value  */
/*   otherwise it will add the symbol into the symbol table                      */
/*   if the string is null then it will return -1                                */

/*********************************************************************************/
int
constval( char *s )
{
  int val = 0, i;

  if( s[0] != '-' ){
    for( i = 0; s[i] != '\0'; i++ )
      val = val * 10 + s[i] - 48;  
  }       // calculating constant value
  else {
    for( i = 1; s[i] != '\0'; i++ )
      val = val * 10 + s[i] - 48; 
    val = -1 * val;
  }
  

  return val;
}


int 
indexof_symtab( char *symbol )       
{
   int i;
   if( strcmp( symbol, "\0" ) )  // if the symbol is null string then return -1
     {

      	
       for( i = 0; i < stab.numsymbols; i++ )
	 if( !strcmp( stab.sym_tab[i], symbol ) ){

 
	    return stab.val_tab[i];
         }
       
   i = stab.numsymbols++; // symbol not found in the symbol table
   stab.sym_tab[i] = ( char * ) malloc( strlen( symbol ) +1 );
   strcpy( stab.sym_tab[i], symbol );//copy the symbol into the symbol table
   stab.val_tab[i] = constval( symbol );    
   return stab.val_tab[i];
     }
   return -1;
}

/*************************************************************************/


void
symbol_for_index( int n , char *sym_value ){
  int i;
  for( i = 0; i < stab.numsymbols; i++ )
    if( stab.val_tab[i] == n ){
      strcpy( sym_value, stab.sym_tab[i] );
    }
}




PRESPLUS readFile ()
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
 fp = fopen(name, "r");
 char dump[100];

  PRESPLUS model;
  model.ctx = mk_context();
  int i,j,k;
  int var_index = 0, place_in;

  fscanf(fp,"\n Number of Places = %d", &model.no_of_places);  
  fscanf(fp,"\n Places ... \n"); 
  for(i=0;i< model.no_of_places;i++) 
  {
    fscanf(fp,"\n Place : %s ",model.places[i].name);
    char name[4];
    fscanf(fp,"\n Associated variable : %s",name);
    int here;
    here = searchForVariable (model, name);

    if (here == -1)
    {	   
     /*** enter this variable name in model.var_table[var_index]
      *  then put "var_index++"  in model.places[place_index].var_index
      *  ***/
     strcpy(model.var_table[var_index].name,name);
     model.places[i].var_index = var_index; 
     var_index++;
    }
    else 
    { /*** put "here" in model.places[place_index].var_index ***/
           model.places[i].var_index = here;
    }
    fscanf(fp,"\n Associated token value:%d",&model.places[i].token_present);
    fscanf(fp,"\n\n") ;
  }
  fscanf(fp,"\n Number of Transitions = %d", &model.no_of_transitions);
  fscanf(fp,"\n Transitions ... \n");
  for(i=0;i<model.no_of_transitions;i++)
  {
    fscanf(fp,"\n Transition : %s",model.transitions[i].name);
    char* guardCondition = (char*)malloc(50*sizeof(char)) ;
    fscanf(fp,"\n Guard conditions:");
    
      fscanf(fp,"%s",guardCondition);
      if(strcmp(guardCondition,"n_g")==0)
        strcpy(guardCondition,"1=1");
      model.transitions[i].guard = ParseExpression(guardCondition);
      model.transitions[i].condition = EXPRNodetoZ3(&model.ctx, model.transitions[i].guard);

    fscanf(fp,"\n Z3 Guard conditions:###%s###",dump);
    fscanf(fp,"%[^\n]s\n",dump);
    fscanf(fp,"\n Number of preset edges : %d",(int*)dump) ;
    fscanf(fp,"\n Preset edge list : ") ;
    for (j=0; j < *(int*)dump; j++)
    {
         fscanf(fp,"%[^\n]s ", dump/*model.edges[model.transitions[i].preset[j]].name*/);
    }
    fscanf(fp,"\n Number of postset edges : %d",(int*)dump) ;
    fscanf(fp,"\n Postset edge list : ") ;
    for (j=0 ; j< *(int*)dump;j++)
    {
         fscanf(fp,"%[^\n]s ", dump /*model.edges[model.transitions[i].postset[j]].name*/);
    }
    fscanf(fp,"\n\n") ;
  } 
  

  
  fscanf(fp,"\n Number of Edges : %d",&model.no_of_edges) ;
  //printf("No of edges %d\n", model.no_of_edges);

  fscanf(fp,"\n Edges ... : ") ;
  for(i=0;i<model.no_of_edges;i++)    
  {
    fscanf(fp,"\n\n Edge : %s ",model.edges[i].name);
    fscanf(fp,"\n Type : %d",&model.edges[i].from_transition);
    //fscanf(fp,"\n %[^\n]s",dump) ;
	//printf("%s",dump);
    
    if(model.edges[i].from_transition==0)
    {
      char tname[4],pname[4];      
      
      fscanf(fp,"\n Connects Place %s to Transition %s",pname,tname);
      //printf("Searching for place %s",pname);
      int pid = searchForPlace(model, pname);
      if(pid!=-1){
        model.edges[i].place = pid;
        int val=++model.places[pid].no_of_postset;
        model.places[pid].postset[val-1]=i;
      }
      else{
        printf("!! place not found in model !!");
        model.edges[i].place = pid;  
      }

      //printf("Searching for transition %s",tname);
      int tid = searchForTransition(model, tname);
      if(tid!=-1){
        model.edges[i].transition = tid;
        int val=++model.transitions[tid].no_of_preset;
        model.transitions[tid].preset[val-1]=i;
      }
      else{
        printf("!! Transition not found in model !!");
        model.edges[i].transition = tid;
      }     
    }
    else
    {
    	char tname[4],pname[4];
      fscanf(fp,"\n Connects Transition %s to Place %s",tname,pname);
      model.edges[i].from_transition=1;
      char expression[50];
      fscanf(fp,"\n Transition function :");
      fscanf(fp," %s ", expression);
      char* eqp=strchr(expression,'=');
      *(eqp)='\0';
      model.edges[i].expr = ParseExpression(eqp+1);
      //parseassignments(expression, model.edges[edge_index].action);
      model.edges[i].action[0].lhs=searchForVariable(model, expression);
      model.edges[i].action[0].rhs=EXPRNodetoZ3(&model.ctx,model.edges[i].expr);
	    *(eqp)='=';
      model.edges[i].expr = ParseExpression(expression);
      
	  //fscanf(fp, "\n Z3 Transition function :###%s###",dump);
	  fscanf(fp,"%[^\n]s",dump);

      int tid = searchForTransition(model, tname);
      if(tid!=-1){
        model.edges[i].transition = tid;
        int val=++model.transitions[tid].no_of_postset;
        model.transitions[tid].postset[val-1]=i;
      }
      else{
        printf("!! Transition not found in model !!");
        model.edges[i].transition = tid;
      }

      int pid = searchForPlace(model, pname);
      if(pid!=-1){
        model.edges[i].place = pid;
        int val=++model.places[pid].no_of_preset;
        model.places[pid].preset[val-1]=i;
      }
      else{
        printf("!! place not found in model !!");
        model.edges[i].place = pid;  
      }

    }

  }

  //fscanf(fp,"\n\n\n Printing initial marking ... %s",dump);
  fscanf(fp,"\n\n\n%[^\n]s",dump);
  //printf("%s",dump);
  
  fscanf(fp,"\n Total number of initially marked places : %d",&model.no_of_places_initially_marked);
  fscanf(fp,"\n Initially marked places are %s ",dump);
  for(i=0; i< model.no_of_places_initially_marked; i++)
  {
  	 char pname[4];
     fscanf(fp," %s ",pname);
     int pid = searchForPlace(model, pname);
      if(pid!=-1){
        model.initial_marking[i]=pid;
      }
      else{
        printf("!! place not found in model !!");
        model.edges[i].place = pid;  
      }

  }
  

  fclose(fp);

/*************************************/ model = connectSetPlaces (model) ;
return model;
}

