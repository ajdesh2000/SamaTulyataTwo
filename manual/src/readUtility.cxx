#include "Utility.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
extern SYMTAB  stab;


/* READ PLACES OF PRES+ MODEL*/

PRESPLUS readplace (PRESPLUS model)
{
  int no_of_places, no_of_variables;
  int place_index, trans_index, var_index;

  printf("\n Enter the number of place : ");
  scanf("%d", &no_of_places);
  model.no_of_places = no_of_places ;

  for(place_index = 0 , var_index = 0 ; place_index < no_of_places; 
		  place_index++)
  { 
    printf("\n Enter the %dth place name : ",place_index);
    scanf("%s", model.places[place_index].name) ;
    int nameIdx ;
    char name[4];
    // char* name = (char*)malloc(sizeof(char));
    printf("\n Enter the name of the variable associated with that place : ");
    getchar();  // dummy read to consume the line feed give earlier in stdin
    scanf("%s",name) ;
    
   /* search for its index in the "model.var_table" */
   int here;
   here = searchForVariable (model, name);

   if (here == -1)
   {	   
     /*** enter this variable name in model.var_table[var_index]
      *  then put "var_index++"  in model.places[place_index].var_index
      *  ***/
     strcpy(model.var_table[var_index].name,name);
     model.places[place_index].var_index = var_index; 
     var_index++;
    }
    else 
    { /*** put "here" in model.places[place_index].var_index ***/
           model.places[place_index].var_index = here;
    }
   //printf("Var index in general is %d Var index is %d  and Variable associated with this place is %s",var_index,model.places[place_index].var_index,model.var_table[model.places[place_index].var_index].name);

  }

  /* Update the number of total variables */
  model.no_of_variables = var_index ; 
  return model ;
}


/* READ TRANSITIONS OF PRES+ MODEL*/

PRESPLUS readtransition(PRESPLUS model)
{ 
   int idx ;
   char* c;
   printf("\n Enter the number of transitions : ");
   scanf("%d",&model.no_of_transitions);

   for (idx = 0; idx < model.no_of_transitions; idx++)
   {
      char* name = (char*)malloc(4*sizeof(char));
      printf("\n Enter the %dth transition name : ",idx);
      scanf("%s",name) ;
       
      /* search for its index in the "model.transitions" 
       * The part is redundant, though I have tried to
       * use model.transitions[idx].name
       * bu here is some eror
       */ 
      if(searchForTransition(model, name) >= 0)
      {
		idx-- ;
                model.no_of_transitions-- ;
		continue ;
      }

     for(int i=0; i< 4; i++)
     {
       model.transitions[idx].name[i]=name[i];
     }         
      //char* expression = (char*)malloc(50*sizeof(char));
      //printf("\n Enter the expression associated with transition : ");
      //scanf("%s",expression);
      //model.transitions[idx].expr = ParseExpression(expression);
      //parseassignments(expression,model.transitions[idx].action);

      char* guardCondition = (char*)malloc(50*sizeof(char)) ;
      printf("\n Enter the guard condition ( if guard conditions are not present type n_g)........: ");
      scanf("%s",guardCondition);
      if(strcmp(guardCondition,"n_g")==0)
        strcpy(guardCondition,"1=1");
      model.transitions[idx].guard = ParseExpression(guardCondition);
      
      //model.transitions[idx].condition=parsecondition(guardCondition,model.transitions[idx].condition);
      // becomes
      model.transitions[idx].condition = EXPRNodetoZ3(&model.ctx, model.transitions[idx].guard);

      /*printf ("\n Enter the number of places in preset : ");
      scanf("%d",&(model.transitions[idx].no_of_preset));

      int presetIdx ;
      int placeIndex ;
      for (presetIdx = 0 ; presetIdx < model.transitions[idx].no_of_preset ; presetIdx++)
      {
	 char* name = (char*)malloc(4*sizeof(char)) ;
	 printf("\n Enter %dth preset name : ", presetIdx);
	 scanf("%s", name);
	// search for its index in the "model.places"

         placeIndex = searchForPlace(model,name) ;
         model.transitions[idx].preset[presetIdx] = placeIndex ; 
	 if(placeIndex == -1)
	 {
		// Error Situation : Need to give an error,
                 //  though the -1 index is already inserted
                 
	 }
       }
       printf ("\n Enter the number of places in postset : ");
       scanf("%d",&(model.transitions[idx].no_of_postset)) ;
       for (presetIdx  = 0; presetIdx < model.transitions[idx].no_of_postset; presetIdx++)
       {
         char* name = (char*)malloc(4*sizeof(char)) ;
         printf("\n Enter %dth postset name : ", presetIdx);         
         scanf("%s",name) ;
         // search for its index in the "model.places"

         placeIndex = searchForPlace(model,name) ;
	 model.transitions[idx].postset[presetIdx]  = placeIndex;
         if(placeIndex == -1)
	 {
		// Error Situation : Need to give an error,
                //   though the -1 index is already inserted
                 
         }
	
       }
	*/
     }
    return model;
}

/* Read Initial marking*/

PRESPLUS readInitialMarking(PRESPLUS model)
{
    int no_of_marked_places ;
    printf("\n Reading the initial marking ...") ;
    printf("\n Enter the number of places containing marking : ") ;
    scanf("%d",&no_of_marked_places) ;
	
    model.no_of_places_initially_marked = no_of_marked_places ;
	
    int idx ;
    int initial_marking_index = 0 ;
    for(idx=0; idx< no_of_marked_places; idx++)
    {
        char* name = (char*)calloc(4,sizeof(char)) ;
        printf(" Enter the name of %dth marked place : ",idx) ;
        scanf("%s",name) ;

          /* search for its index in the "model.places" */

	  int placeIndex = searchForPlace(model, name) ;
           
	   if(placeIndex >= 0 )
	   {
		printf("In readInitialMarking: placeIndex=%d\n",placeIndex);	   	
		model.initial_marking[initial_marking_index++] = placeIndex ;
	   	model.places[placeIndex].token_present = 1 ;
           }
           else
           {
	  	/* Error situation */
           }
    }
    return model ;
}

PRESPLUS readedge(PRESPLUS model){
  int num_edges;
  

  printf("\n\n Enter the number of edges: ");
  scanf("%d", &num_edges);
  model.no_of_edges = num_edges;
  printf("\n Number of edges is: %d\n", num_edges);
  for(int edge_index=0;edge_index<num_edges;edge_index++){
    printf("\n Enter edge #%d name: ", edge_index);
    scanf("%s", model.edges[edge_index].name);

    

    printf("\n Is this edge from a place or a transition? (p/t): ");
    char c;
    scanf(" %c", &c);
    if(c=='p'){
      model.edges[edge_index].from_transition=0;
      printf("\n Enter place name: ");
      char pname[4];
      scanf("%s",pname);
      int pid = searchForPlace(model, pname);
      if(pid!=-1){
        model.edges[edge_index].place = pid;
        int val=++model.places[pid].no_of_postset;
        model.places[pid].postset[val-1]=edge_index;
      }
      else{
        printf("!! place not found in model !!");
        model.edges[edge_index].place = pid;  
      }

      printf("\n Enter transition name: ");
      char tname[4];
      scanf("%s", tname);
      int tid = searchForTransition(model, tname);
      if(tid!=-1){
        model.edges[edge_index].transition = tid;
        int val=++model.transitions[tid].no_of_preset;
        model.transitions[tid].preset[val-1]=edge_index;
      }
      else{
        printf("!! Transition not found in model !!");
        model.edges[edge_index].transition = tid;
      }
    

    }


    else{
      model.edges[edge_index].from_transition=1;

      char expression[50];
      printf("\n Enter the expression associated with edge in the format varname=expression: ");
      scanf(" %s", expression);
      char* eqp=strchr(expression,'=');
      *(eqp)='\0';
      model.edges[edge_index].expr = ParseExpression(eqp+1);
      //parseassignments(expression, model.edges[edge_index].action);
      printf("Im looking for variable '%s' and its index is  %d",expression,searchForVariable(model, expression));
      model.edges[edge_index].action[0].lhs=searchForVariable(model, expression);
      model.edges[edge_index].action[0].rhs=EXPRNodetoZ3(&model.ctx,model.edges[edge_index].expr);
	    *(eqp)='=';
      model.edges[edge_index].expr = ParseExpression(expression);
      
      printf("\n Enter transition name: ");
      char tname[4];
      scanf("%s", tname);
      int tid = searchForTransition(model, tname);
      if(tid!=-1){
        model.edges[edge_index].transition = tid;
        int val=++model.transitions[tid].no_of_postset;
        model.transitions[tid].postset[val-1]=edge_index;
      }
      else{
        printf("!! Transition not found in model !!");
        model.edges[edge_index].transition = tid;
      }

      printf("\n Enter place name: ");
      char pname[4];
      scanf("%s",pname);
      int pid = searchForPlace(model, pname);
      if(pid!=-1){
        model.edges[edge_index].place = pid;
        int val=++model.places[pid].no_of_preset;
        model.places[pid].preset[val-1]=edge_index;
      }
      else{
        printf("!! place not found in model !!");
        model.edges[edge_index].place = pid;  
      }

    }
//    printf("Number of edges is still %d",model.no_of_edges);
  }

  return model;  
}

PRESPLUS initialiseToZero(PRESPLUS model)
{
	int i=0,prenum=0,postnum=0,j=0,k=0,val=0;
	for(i=0;i<model.no_of_places;i++)
	{
		model.places[i].no_of_preset=0;
		model.places[i].no_of_postset=0;
	}
	for(i=0;i<model.no_of_transitions;i++)
	{
		model.transitions[i].no_of_preset=0;
		model.transitions[i].no_of_postset=0;
	}
	return model;
}


PRESPLUS connectSetPlaces(PRESPLUS model)
{
	//does nothing. Left temporarily to prevent compilation errors during the gradual shift to SamaTulyataTwo
	return model;
}

