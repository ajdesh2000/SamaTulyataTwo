#include<stdio.h>
void main()
{
int current_state,newstate,newHL,newFL,cars;
int timeOutL,timeOutS,newST,FarmL,state,HiWay,StartTimer;

if(current_state==2)
{
    newHL=6;
    newFL=4;
    if(cars==0||timeOutL==1)
    {
        newstate=6;
        newST=1;
    }
    else
    {
        newstate=2;
        newST=0;
    }
}
if(current_state==6)
{
    newHL=6;
    newFL=2;
    if(timeOutS=1)
    {
        newstate=0;
        newST=1;
    }
    else
    {
        newstate=6;
        newST=0;
    }
}
if(current_state==7)
{
    newHL=0;
    newFL=0;
    
    
    newstate=0;
    newST=0;
}   
state=newstate;
HiWay=newHL;
FarmL=newST;
StartTimer=newST;
    
    printf ("%d", StartTimer); // assumed to be the output
    
    
    


}




