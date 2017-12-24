/******************************
auth:wxc
data:2017-12-22
com:新建工程，初步完成AT指令的解析和写入返回



*******************************/
#include <AT.h>
typedef ATDat*(func)(ATCmd *cmd);
typedef struct AtData
{
    int count;
    char data[100];
}ATDat;
typedef struct AtCommon
{
    int index;
    char *common;
    char *replay;
    char *data;
    int timeout;
    func *backcallfunc;
}ATCmd;
ATCmd *head;
char *gCmdFlag;
int ARRAY_COUNT = 0;
void initAtCmd(ATCmd *dat)
{
    if(dat != null)
    {
        ARRAY_COUNT = ARRAY(dat);
        head = dat;
        gCmdFlag = (char *)malloc(ARRAY_COUNT);

    }
}

void closeAtCmd()
{
    head = 0;
    if(gCmdFlag != null)
    {
        free(gCmdFlag);
    }
}
void sendAtCmd(int index)
{
    UsartSendCmd(head[index].common);
    gCmdFlag[index] = SEND;
}
void input(char ch)
{
    static int index = 0;
    static char buff[500] = {0};
    buff[index++] = ch;
    if(ch == '\n' && buff[index - 2] == '\r')
    {
        //数据接收完成，进行解析
        for(int i = 0 ; i < ARRAY_COUNT ; i ++)
        {
            if(gCmdFlag[i] == SEND)
            {

            }
        }
    }
    else
    {

    }

}
void callBackfunc(char ch)
{
    input(ch);
}






