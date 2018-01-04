
/******************************
auth:wxc
data:2017-12-25
com:创建sim800文件，用来设计sim800的一系列函数
todo: at+caq   at+creg   ATE  tcp   udp 透传，非透传,CCID  ........

auth:wxc
data:2017-12-27
com:添加指令结构体，实现公用函数，添加了tcp，udp创建，发送，读取函数，
todo: 一个函数实现发送和接收，目前指令接收的模式还有点问题，透传基本可以使用
*******************************/

#include "../AT/AT.h"
#include "SIM800.h"


//typedef struct AtCommon
//{
//    int index;
//    char *common;
//    char replay[COUNT_REVDATA];
//    char *data;
//    int timeout;
//    func backcallfunc;
//    char revflag;
//    char rpybuff[10];
//}ATCmd;
ATCmd gArrCmd[] =
{
    {0,"ATE","",0,20,0,0,"OK"},
    {1,"AT+CSQ","+CSQ:24,99\r\nOK\r\n",0,20,0,0,"OK"},
    {2,"AT+creg","",0,20,0,0,"OK"},
    {3,"AT+ccid","",0,20,0,0,"OK"},
    {4,"AT","",0,20,0,0,"OK"},
    {5,"AT+CPIN","",0,20,0,0,"READY"},
    {6,"AT+CGATT","",0,20,0,0,"OK"},
    {7,"AT+CSTT","",0,20,0,0,"OK"},
    {8,"AT+CIFSR","",0,20,0,0,"\r\n"},
    {9,"AT+CIPSTART","",0,20,0,0,"OK"},
    {10,"AT+CIPMODE","",0,20,0,0,"OK"}, //1是透传 0是非透传
    {11,"AT+CIPCLOSE","",0,20,0,0,"OK"},
    {12,"AT+CIPSEND","",0,20,0,0,">"},
    {13,"AT+CIPSRIP","",0,20,0,0,"OK"},
    {14,"AT+CIP","",0,20,0,0,">"},
};



/** \brief backcallfunc  回调函数，指令结束之后，会调用这个函数进行数据反馈
 *
 * \param mode 返回的模式
 * \param index 当前执行的指令
 * \return
 *
 */
void backcallfunc(int mode,int index)
{
    switch(mode)
    {
    case OK:
        break;
    case ERROR:
        break;

    default : break;

    }
}

//初始化
void initSim800()
{
    initAtCmd(gArrCmd,backcallfunc);
}
char isExecOK(char *buff)
{
    if(buff != null)
    {
        if(strstr(buff,"OK") != null)
        {
            return OK;
        }
    }
    return ERROR;
}
/** \brief 获取指令名称后面的数据
 *
 * \param
 * \param
 * \return
 *
 */

char *getAtValue(char *data)
{
    char *dataIndex;
    if(data != null && (dataIndex = strchr(data,':')) != null)
    {
        return dataIndex + 1;  //冲 ':'往后移一位
    }
    return null;
}
/** \brief 获取各个数据   getAtValueforString
 *
 * \param data 数据字符串
 * \param len 有几个数据
 * \return 返回字符串
 *
 */
 static char * getAtValueforString(char *data,int len)
 {
     static char revbuff[10][50] = {{0}};
     static int buffIndex = 0;
     int i = 0;
     char *tempData;
     char buff[50] = {0};
     if(data == null)
     {
        buffIndex ++;
        return revbuff[buffIndex];
     }
     else
     {
        for(int j = 0 ; j < 10 ; j ++)
        {
            memset(revbuff[j],0,50);
        }
        buffIndex = 0;
     }
     for(i = 0 ; i < len -1 ; i ++)
     {
        tempData = strchr(data,',');
        memset(buff,0,sizeof(buff));
        strncpy(buff,data,tempData-data);
        memcpy(revbuff[i],buff,sizeof(buff));
        data = tempData+1;   //tempData目前指再 ',' 字符这里
     }
     memset(buff,0,sizeof(buff));
     tempData = strchr(data,'\r');
     strncpy(buff,data,tempData-data);
     memcpy(revbuff[i],buff,sizeof(buff));
     return revbuff[buffIndex];
 }



/** \brief 获取各个数据
 *
 * \param  输入的字符串
 * \param  有几个数据
 * \return 返回整形数组
 *
 */
 static int * getAtValueforint(char *data,int len)
 {
     static int revbuff[50] = {0};
     int i = 0;
     char *tempData;
     char buff[50] = {0};
     if(data == null)
     {
        return null;
     }
     for(i = 0 ; i < len -1 ; i ++)
     {
        tempData = strchr(data,',');
        memset(buff,0,sizeof(buff));
        strncpy(buff,data,tempData-data);
        revbuff[i] = atoi(buff);

        data = tempData+1;   //tempData目前指再 ',' 字符这里
     }
    memset(buff,0,sizeof(buff));
    tempData = strchr(data,'\r');
    strncpy(buff,data,tempData-data);
    revbuff[i] = atoi(buff);
    return revbuff;
 }

/** \brief getAtCommomInt
 *
 * \param  需要发送的指令，和需要返回的参数的个数
 * \param
 * \return
 *
 */

int *getAtCommomInt(int CommomId,int parmLen)
{
    static int *arrData;
    //CLEAN_ARR(arrCsqData,2);
    sendAtCmd(CommomId,0);
    print("had send cmd \r\n");
    while(getATrevFlag(CommomId) != WAIT)
    {
        //接收完成
        print("rev success \r\n");
        char *data = getATrevdata(CommomId);
        if(isExecOK(data) == OK)
        {
            if((data = getAtValue(data)) == null)
            {
                break;
            }
            //接下来获取csq的数据
            arrData = getAtValueforint(data,parmLen);
        }
        //cleanATrevFlag(CommomId);
    }
    return arrData;
}
/** \brief getAtCommomString
 *
 * \param  需要发送的指令，和需要返回的参数的个数
 * \param
 * \return
 *
 */
static char **atRevData;
char **getAtCommomString(int CommomId,int parmLen)
{
    //CLEAN_ARR(arrCsqData,2);
    sendAtCmd(CommomId,0);
    print("had send cmd \r\n");
    while(getATrevFlag(CommomId) != WAIT)
    {
        //接收完成
        print("rev success \r\n");
        char *data = getATrevdata(CommomId);
        if(isExecOK(data) == OK)
        {
            if((data = getAtValue(data)) == null)
            {
                break;
            }
            //接下来获取csq的数据
            atRevData[0] = getAtValueforString(data,parmLen);
            for(int i = 1 ; i < parmLen ; i ++)
            {
                atRevData[i] = getAtValueforString(null,parmLen);
            }
        }
        //cleanATrevFlag(CommomId);
    }
    return atRevData;
}
/**\ getAtCommom
 *
 * \param 发送指令，和参数，专门用于没有返回值的指令进行处理
 * \param
 * \return
 *
 */
int getAtCommom(int CommomId,char *parm)
{
    setAtParm(CommomId,parm);
    sendAtCmd(CommomId,0);

    print("had send cmd \r\n");
    while(getATrevFlag(CommomId) != WAIT)
    {
        //接收完成
        print("rev success \r\n");
        char *data = getATrevdata(CommomId);
        return getATrevFlag(CommomId);
    }
}
/**\ cifsr
 *
 * \param 定制化CIFSR的指令
 * \param
 * \return
 *
 */
 char *getatCifsrforIPAddress(int commomdId)
 {
    static char ipAdress[50];
    sendAtCmd(commomdId,0);
    print("had send cmd \r\n");
    while(getATrevFlag(commomdId) != WAIT)
    {
        //接收完成
        print("rev success \r\n");
        char *data = getATrevdata(commomdId);
        if(strstr(data,"ERROR") != null)
        {
            return null;
        }
        //接下来获取csq的数据
        memcpy(ipAdress,data,sizeof(ipAdress));
        //cleanATrevFlag(CommomId);
    }
    return ipAdress;
 }

