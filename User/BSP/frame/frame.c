#include <includes.h>
#include "frame.h"


void  AppTaskWriteFrame ( void * p_arg )
{
    OS_ERR      err;
    while (DEF_TRUE) 
    {                            //�����壬ͨ��д��һ����ѭ��

        OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //��������������

    }
}

void  AppTaskReadFrame ( void * p_arg )
{
    OS_ERR      err;
    while (DEF_TRUE) 
    {                            //�����壬ͨ��д��һ����ѭ��

        OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //��������������

    }
}
