#ifndef __AT_H__

#define __AT_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../commom.h"
#define COUNT_REVDATA    1024

typedef void (*callFunc)(int mode,int index);
typedef char (*func)(char *revDatam, int index);
typedef struct AtCommon
{
    int index;
    char *common;
    char replay[COUNT_REVDATA];
    char *parm;
    int timeout;
    func backcallfunc;
    char revflag;
    char *rpybuff;
}ATCmd;

enum
{
    REVDATA = 1,
    ATMODE,
}enumAtMode;



extern void initAtCmd(ATCmd *dat,callFunc *func);
extern void callBackfunc(char ch,int time);
extern char getATrevFlag(int index);
extern void cleanATrevFlag(int index);
extern void sendAtCmd(int index,int time);
extern void closeAtCmd(void );
extern char *getATrevdata(int index);
extern void setAtParm(int index,char *parm);
extern void getRevData(char *data);


extern void uartSendstring(char *data);
extern uint getTick(void);
#endif
