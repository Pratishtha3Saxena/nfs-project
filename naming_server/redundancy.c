#include "basic_header.h"
#include "header.h"
#include "function_def.h"
#include "macros.h"
#include "S_centre.h"
int* redundancy(int ownindex)
{
    int min;
    int min1;
    int index;
    int index1;
    int* a=malloc(sizeof(int)*2);
    while (1)
    {
        min = 900000;
        min1 = 900000;
        for (int i = 0; i < Max_SS_no; i++)
        {
            if (i == ownindex)
            {
                continue;
            }
            if (STORAGE_SERVER[i].status != EMPTY)
            {
                if (min > backupcount[i])
                {
                    min1 = min;
                    min = backupcount[i];
                    index1 = index;
                    index = i;
                }
                else if (min1 > backupcount[i])
                {
                    min1 = backupcount[i];
                    index1 = i;
                }
            }
        }
        if (min != 900000 && min1 != 900000)
        {
            break;
        }
    }
    backupcount[index]++;
    backupcount[index1]++;
    a[0]= index;
    a[1]= index1;
    return a;

}