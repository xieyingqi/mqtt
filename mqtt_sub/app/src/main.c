#include "includes.h"

EP_LISTEN_T gMainLoop;
SOCKET_IPC_T gIpc;
TIMER_STRU_T gTimer1, gTimer2;
CMD_LIST_T cmdList;

extern void MQTT_Client_Init(void);
extern void MQTT_Client_Destroy(void);

CMD_PARA_T cmdParaList[] = 
{
	{"id",        TYPE_TEXT, cmdList.id},
	{"log_level", TYPE_TEXT, cmdList.log_level},
	{"type",      TYPE_INT,  &cmdList.type},
};

void cbTimer1(sigval_t i)
{
	IPC_MSG_T send_msg;
	
	strcpy(send_msg.buf, "hello");

	if(strcmp(cmdList.id, "recv") != 0)
	{
		sendIpc(&gIpc, "recv", send_msg);
	}
}

void cbTimer2(sigval_t i)
{
	logPrintf("timer2");
}

void cbSock(IPC_MSG_T *msg)
{
	logPrintf("rcv %d data from:%s msg:%s", msg->cnt, msg->id, msg->buf);
}

int main(int argc, char *argv[])
{
	parseCmdPara(argc, argv, cmdParaList, sizeof(cmdParaList) / sizeof(CMD_PARA_T));
	setLogLevel(cmdList.log_level);
	
	epollCreate(&gMainLoop);

	creatIpc(&gMainLoop, &gIpc, cmdList.id);
	setIpcCallBack(&gIpc, cbSock);
	
	// addTimer(&gTimer1, 1, cbTimer1);
	// startTimer(&gTimer1);

	// addTimer(&gTimer2, 5, cbTimer2);
	// startTimer(&gTimer2);
	
	MQTT_Client_Init();
	epollListenLoop(&gMainLoop);

	return 0;
}