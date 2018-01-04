#ifndef __SIM800__H
#define __SIM800__H
#include "../commom.h"

enum
{
    ATE = 0,
    CSQ = 1,
    CREG,
    CCID,
    AT,
    CPIN,
    CGATT,
    CSTT = 7,
    CIFSR,
}enumATcommom;

#define CLEAN_ARR(__X__ , __N__)    (memset((__X__),0,sizeof(__X__)))

#define AT_COMMON_COUNT   100



//extern ATCmd gArrCmd[AT_COMMON_COUNT];
extern void initSim800(void);
extern int *getAtCommomCsq(int CommomId);

#endif // __SIM800__H

