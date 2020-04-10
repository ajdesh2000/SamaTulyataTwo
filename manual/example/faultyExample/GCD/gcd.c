 #include<stdio.h>
void main()
{
    int y1, y2, res, yout, i;
    res=1;
    for(i=0;y1!=y2;i++)
    {
        if(y2%2==0)
                y2=y2/2;
                else
                    y2=y2-y1;
    }
    res=res*y1;
    yout=res;
    printf ("%d", yout);
}



    
