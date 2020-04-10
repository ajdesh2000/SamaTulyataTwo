#include "Utility.h"

PRESPLUS readKBDF()
{
  
  PRESPLUS model;

  model = readplace (model); 
  // reads places and the variables associated with them
  model = readtransition (model);
 
  model = initialiseToZero(model) ;

  model = readedge(model);  

  model = readInitialMarking (model) ;
  
  printf("Number of edges= %d",model.no_of_edges);

  return model;
} 
