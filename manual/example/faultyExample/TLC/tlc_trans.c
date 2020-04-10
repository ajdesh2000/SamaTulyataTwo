#include <stdio.h>

// extern int main(void);
// extern void _main();

void main()
{
 int current_state;
 int newstate;
 int newHL;
 int newFL;

 int returnVar_main;
 
  if (sT6_36)
   {
    sT9_40 = ((sT14_40) || (sT13_40));
    newHL = 6;
    newFL = 4;
    sT7_40 = sT13_40;
    sT8_40 = sT14_40;
    if (sT9_40)
     {
      newstate = 6;
      newST = 1;
      sT12_66 = (current_state == 7);
     } /* sT9_40     */
    else
     {
      newstate = 2;
      newST = 0;
      sT12_66 = (current_state == 7);
     }  /* end of if-else (sT9_40)*/
   } /* sT6_36   */
  else
   {
    sT12_66 = (current_state == 7);
   }  /* end of if-else (sT6_36)*/
 

