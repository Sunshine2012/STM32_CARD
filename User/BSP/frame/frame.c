#include <includes.h>
#include "frame.h"


void  AppTaskWriteFrame ( void * p_arg )
{
    OS_ERR      err;
    while (DEF_TRUE) 
    {                            //任务体，通常写成一个死循环

        OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //不断阻塞该任务

    }
}

void  AppTaskReadFrame ( void * p_arg )
{
    OS_ERR      err;
    while (DEF_TRUE) 
    {                            //任务体，通常写成一个死循环

        OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //不断阻塞该任务

    }
}
