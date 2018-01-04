
#ifndef __COMMON__H
#define __COMMON__H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define ARRAY(__X__)   (sizeof(__X__) / sizeof(__X__[0]))

#define uchar unsigned char
#define uint unsigned int
#define SEND        1
#define PENDING     2
#define WAIT        0

#define ERROR       1
#define OK          2
#define null        ((void *)0)

extern void print(char *data);


#endif // __COMMON__H
