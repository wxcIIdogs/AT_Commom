#include <stdio.h>
#include <stdlib.h>
#include "AT/AT.h"
#include "SIM800/SIM800.h"
int main()
{
    char revdata[1024] ={"0"};
    initSim800();
    print("init finash\r\n");
    int *data = getAtCommom(CSQ);
    printf("%d,%d",data[0],data[1]);
}
void uartSendstring(char *data)
{
    printf("%s\r\n",data);
}
uint getTick()
{
    return 1;
}
