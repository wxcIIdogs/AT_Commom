/******************************
auth:wxc
data:2017-12-22
com:新建工程，初步完成AT指令的解析和写入返回
todo:

auth:wxc
data:2017-12-25
com:添加结构体并且完成发送数据，接收解析数据，获取时间戳等函数
todo:

auth:wxc
data:2017-12-27
com:添加参数设置函数，并且更具参数有误，发送不同的指令,添加sim800的数据读取函数
todo:


*******************************/
#include "AT.h"

//typedef struct AtData
//{
//    int count;
//    char data[COUNT_REVDATA];
//}ATDat;

static ATCmd *head;
static char *gArrCmdFlag;
static int ARRAY_COUNT = 0;
static uint g_timecount = 0;
static int ATNowMode = 0;
static callFunc *backcallfunc;
void initAtCmd(ATCmd *dat,callFunc *func)
{
    if(dat != null)
    {
        ARRAY_COUNT = ARRAY(dat);
        head = dat;
        gArrCmdFlag = (char *)malloc(ARRAY_COUNT);

        backcallfunc = func;
    }
}

int getAtNowMode()
{
    return ATNowMode;
}
void setAtNowMode(int mode)
{
    ATNowMode = mode;
}
void closeAtCmd()
{
    head = 0;
    if(gArrCmdFlag != null)
    {
        free(gArrCmdFlag);
    }
}

void UsartSendCmd(char* dat)
{
    if(dat != null)
    {
        uartSendstring(dat);
    }
}
char *getATrevdata(int index)
{
    if(&head[index] != null)
    {
        return head[index].replay;
    }
}
/** \brief  setAtParm
 *
 * \param index:下标 ,
 * \param parm 参数
 * \return void
 *
 */

void setAtParm(int index,char *parm)
{
    if(parm == null && &head[index] == null)
    {
        return;
    }
    head[index].parm = parm;
}
/** \brief sendAtCmd
 *
 * \param index 下标，知道是那个指令在执行
 * \param
 * \return void
 *
 */

void sendAtCmd(int index,int time)
{
    if(&head[index] != null)
    {
        char buff[100] = {0};
        g_timecount = time;
        setAtNowMode(ATMODE); //设置为指令发送模式
        cleanATrevFlag(index); // 清空接收状态和当前处于状态
        if(head[index].parm != 0 && head[index].parm != "" ) //区分有没有参数，进行格式化组合
        {
            sprintf(buff,"%s=%s\r\n",head[index].common,head[index].parm);
        }
        else
        {
            sprintf(buff,"%s\r\n",head[index].common);
        }
        gArrCmdFlag[index] = SEND;
        head[index].revflag = ERROR;
        UsartSendCmd(buff);
    }
}
static char revDatabuff[COUNT_REVDATA];
static int readCount = 0;
void getRevData(char *data)
{
    if(data!= null)
    {
        //把接收到的数据一次全部拷贝走. 这个函数不好用
        char temp_buff[1024];
        memcpy(temp_buff,revDatabuff,readCount);

        memcpy(data,temp_buff,readCount);

        memset(revDatabuff,0,readCount);
        readCount = 0;


    }
}

void input(char ch)
{
    //  dadsadadasdasds\r\n
    // \r\n OK  \r\n
    static int index = 0;
    static uchar stcRevbuff[COUNT_REVDATA/2] = {0};

    stcRevbuff[index++] = ch;

    for(int i = 0 ; i < ARRAY_COUNT ; i ++)
    {
        //收到的是什么数据，不在这里进行判断 这里只判断是否返回
        if(gArrCmdFlag[i] == SEND && strstr(stcRevbuff,head[i].rpybuff)!= null)
        {
            //处于发送状态，并且返回值里面有指定的结束符
            strcpy(head[i].replay,(char *)stcRevbuff);
            memset(stcRevbuff,0,index);
            index = 0;
            gArrCmdFlag[i] = PENDING;
        }//if(gCmdFlag[i] == SEND)
    }//for
}
//清空指令执行之后的状态信息
void cleanATrevFlag(int index)
{
    if(&head[index] != null)
    {
        gArrCmdFlag[index] = WAIT;
        head[index].revflag = WAIT;
    }
}
//返回指令执行结果，OK 或 ERROR
char getATrevFlag(int index)
{
    if(&head[index] != null)
    {
        return head[index].revflag;
    }
    return -1;
}

void callBackfunc(char ch,int time)
{

    if(getAtNowMode() == REVDATA)
    {
        //把接受到的是数据存储到指定的地方
        revDatabuff[readCount++] = ch;
        if(readCount > COUNT_REVDATA)
        {
            readCount = 0;
        }
    }
    else
    {
        input(ch);
    }
    //判断指令执行的状态
    for(int i = 0 ; i < ARRAY_COUNT ; i++)
    {
        if(time - g_timecount > head[i].timeout && gArrCmdFlag[i] == SEND)
        {
            head[i].revflag = ERROR;
            gArrCmdFlag[i] = WAIT;
            (*backcallfunc)(head[i].revflag,i);
            return;
        }
        if(gArrCmdFlag[i] == PENDING)
        {
            //如果是等待处理的指令
            head[i].revflag = OK;
            gArrCmdFlag[i] = WAIT;
            (*backcallfunc)(head[i].revflag,i);
            break;
        }
    }
}






