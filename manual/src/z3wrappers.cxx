#include<stdarg.h>
#include<memory.h>
#include<setjmp.h>
#include<z3.h>
#include "Utility.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG_Z3_CALLS

#ifdef LOG_Z3_CALLS
#define LOG_MSG(msg) Z3_append_log(msg)
#else
#define LOG_MSG(msg) ((void)0)
#endif




//Z3_ast x, y;


/**
   \defgroup capi_ex C API examples
*/
/*@{*/
/**
   @name Auxiliary Functions
*/
/*@{*/

/**
   \brief exit gracefully in case of error.
*/
void exitf(const char* message)
{
  fprintf(stderr,"BUG: %s.\n", message);
  exit(1);
}

/**
   \brief exit if unreachable code was reached.
*/
void unreachable()
{
    exitf("unreachable code was reached");
}

/**
   \brief Simpler error handler.
 */
void error_handler(Z3_context c, Z3_error_code e)
{
    printf("Error code: %d\n", e);
    exitf("incorrect use of Z3");
}

static jmp_buf g_catch_buffer;
/**
   \brief Low tech exceptions.

   In high-level programming languages, an error handler can throw an exception.
*/
void throw_z3_error(Z3_context c, Z3_error_code e)
{
    longjmp(g_catch_buffer, e);
}

/**
   \brief Error handling that depends on checking an error code on the context.

*/

void nothrow_z3_error(Z3_context c, Z3_error_code e) {
    // no-op
}

/**
   \brief Create a logical context.

   Enable model construction. Other configuration parameters can be passed in the cfg variable.

   Also enable tracing to stderr and register custom error handler.
*/
Z3_context mk_context_custom(Z3_config cfg, Z3_error_handler err)
{
    Z3_context ctx;

    Z3_set_param_value(cfg, "model", "true");
    ctx = Z3_mk_context(cfg);
    Z3_set_error_handler(ctx, err);

    return ctx;
}

Z3_solver mk_solver(Z3_context ctx)
{
  Z3_solver s = Z3_mk_solver(ctx);
  Z3_solver_inc_ref(ctx, s);
  return s;
}

void del_solver(Z3_context ctx, Z3_solver s)
{
  Z3_solver_dec_ref(ctx, s);
}

/**
   \brief Create a logical context.

   Enable model construction only.

   Also enable tracing to stderr and register standard error handler.
*/
Z3_context mk_context()
{
    Z3_config  cfg;
    Z3_context ctx;
    cfg = Z3_mk_config();
    ctx = mk_context_custom(cfg, error_handler);
    Z3_del_config(cfg);
    return ctx;
}

/**
   \brief Create a logical context.

   Enable fine-grained proof construction.
   Enable model construction.

   Also enable tracing to stderr and register standard error handler.
*/
Z3_context mk_proof_context() {
    Z3_config cfg = Z3_mk_config();
    Z3_context ctx;
    Z3_set_param_value(cfg, "proof", "true");
    ctx = mk_context_custom(cfg, throw_z3_error);
    Z3_del_config(cfg);
    return ctx;
}

/**
   \brief Create a variable using the given name and type.
*/
Z3_ast mk_var(Z3_context ctx, const char * name, Z3_sort ty)
{
    Z3_symbol   s  = Z3_mk_string_symbol(ctx, name);
    return Z3_mk_const(ctx, s, ty);
}

/**
   \brief Create a boolean variable using the given name.
*/
Z3_ast mk_bool_var(Z3_context ctx, const char * name)
{
    Z3_sort ty = Z3_mk_bool_sort(ctx);
    return mk_var(ctx, name, ty);
}

/**
   \brief Create an integer variable using the given name.
*/
Z3_ast mk_int_var(Z3_context ctx, const char * name)
{
    Z3_sort ty = Z3_mk_int_sort(ctx);
    return mk_var(ctx, name, ty);
}

/**
   \brief Create a Z3 integer node using a C int.
*/
Z3_ast mk_int(Z3_context ctx, int v)
{
    Z3_sort ty = Z3_mk_int_sort(ctx);
    return Z3_mk_int(ctx, v, ty);
}

/**
   \brief Create a real variable using the given name.
*/
Z3_ast mk_real_var(Z3_context ctx, const char * name)
{
    Z3_sort ty = Z3_mk_real_sort(ctx);
    return mk_var(ctx, name, ty);
}

/**
   \brief Create the unary function application: <tt>(f x)</tt>.
*/
Z3_ast mk_unary_app(Z3_context ctx, Z3_func_decl f, Z3_ast x)
{
    Z3_ast args[1] = {x};
    return Z3_mk_app(ctx, f, 1, args);
}

/**
   \brief Create the binary function application: <tt>(f x y)</tt>.
*/
Z3_ast mk_binary_app(Z3_context ctx, Z3_func_decl f, Z3_ast x, Z3_ast y)
{
    Z3_ast args[2] = {x, y};
    return Z3_mk_app(ctx, f, 2, args);
}





Z3_ast  EXPRNodetoZ3(Z3_context* ctxp, EXPR expr)
{
  //printf("I have entered the function at least");
  
  EXPRNODE tmp = *expr ;
  
  if (tmp.type == 0)
  {
    Z3_ast temp[2];
    temp[0]=EXPRNodetoZ3(ctxp,expr->left);
    temp[1]=EXPRNodetoZ3(ctxp,expr->right);
    switch(tmp.value)
    {
      case 1: //printf("&");
      return Z3_mk_and(*(ctxp),2,temp);
      break;
      case 2: //printf(">");
      return Z3_mk_gt(*(ctxp),temp[0],temp[1]);
      break;
      case 3: //printf("<");
      return Z3_mk_lt(*(ctxp),temp[0],temp[1]);
      break;
      case 4: //printf("|");
      return Z3_mk_or(*(ctxp),2,temp);
      break;
      case 5: //printf(">=");
      return Z3_mk_ge(*(ctxp),temp[0],temp[1]);
      break;
      case 6: //printf("<=");
      return Z3_mk_le(*(ctxp),temp[0],temp[1]);
      break;
      case 7: //printf("=");
      return Z3_mk_eq(*(ctxp),temp[0],temp[1]);
      break;
      case 8: //printf("!=");
      return Z3_mk_not(*(ctxp),Z3_mk_eq(*(ctxp),temp[0],temp[1]));
      break;
      case 9: //printf("+");
      return Z3_mk_add(*(ctxp),2,temp);
      break;
      case 10: //printf("-"); 
      return Z3_mk_sub(*(ctxp),2,temp);
      break;
      case 11: //printf("*"); 
      return Z3_mk_mul(*(ctxp),2,temp);
      break;
      case 12: //printf("/");
      return Z3_mk_div(*(ctxp),temp[0],temp[1]);
      break;
      case 13: //printf("_");
      break;
     }
   }
   else if(tmp.type ==1)
   {
     //printf(" (%c )",tmp.value);
     Z3_sort ty = Z3_mk_int_sort(*ctxp);
     char name[2];
     name[0]=tmp.value;
     name[1]='\0';
     return mk_var(*ctxp, name, ty);
   }
   else//(tmp.type == 2)
  {   
     //printf(" (%d) ",tmp.value);
     Z3_sort ty = Z3_mk_int_sort(*ctxp);
    return Z3_mk_int(*ctxp, tmp.value, ty);
  }
  
  //Z3_sort ty = Z3_mk_int_sort(mk_context());
     /*char name[2];
     name[0]=tmp.value;
     name[1]='\0';
     mk_var(*ctxp, name, ty);*/
}  