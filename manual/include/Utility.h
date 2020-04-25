#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include <z3.h>

extern int drivemenu() ;
extern PRESPLUS readKBDF() ;
extern int printpresplus(PRESPLUS ) ;
extern int  writeFile (PRESPLUS );
extern PRESPLUS  readFile (FILE*);
extern PRESPLUS readplace(PRESPLUS ) ;
extern PRESPLUS readtransition(PRESPLUS ) ;
extern PRESPLUS readInitialMarking(PRESPLUS ) ;
extern PRESPLUS readedge(PRESPLUS);
extern void findEquivalent(PRESPLUS m0,PRESPLUS m1);

extern PRESPLUS connectSetPlaces(PRESPLUS ) ;
extern PRESPLUS initialiseToZero(PRESPLUS ) ;

//extern FSMD symbolicSimulate (PRESPLUS, FSMD, char*);
//extern interFSM createFSM (PRESPLUS, interFSM);
//extern int** retTrans(int*, int);
//extern FSMD createStaticEntities(PRESPLUS, char*, char* );
//extern int symbolicSimulate(PRESPLUS *, FSMD *);
//extern int writeFsmd (FSMD * );

extern int searchForVariable(PRESPLUS , char*);
extern int searchForPlace(PRESPLUS,char*) ;
extern int searchForTransition(PRESPLUS,char*) ;
extern  void DecompilingEXPRNode(EXPR );
extern  void DecompilingExpr(EXPR );
extern void Initial(PRESPLUS *model);
extern EXPR ParseExpression(char*) ;

extern CUTPOINT findcutpoints(PRESPLUS model, CUTPOINT cp);
extern PATHSET ConstructAllPaths(PRESPLUS *model);
extern void visualize(PRESPLUS model);

extern Z3_context mk_context();

extern Z3_ast EXPRNodetoZ3(Z3_context* ctxp,EXPR expr);

#endif 
