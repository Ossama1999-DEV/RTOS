#include "T6.h"

void tache6(void)
{
    unsigned char n;
    while(1)
    {
        n=lecture_normale(badge);
        if (n>0)
        {
            if(n<10)
            {
                n_octet_badge=n;
            }
            else
            {
                n_octet_badge=0;
            }
        }
        else
        {
            n_octet_badge=0;
        }
    }
}
