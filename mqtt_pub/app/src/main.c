#include "includes.h"

EP_LISTEN_T gMainLoop;
SOCKET_IPC_T gIpc;
TIMER_STRU_T gTimer1, gTimer2;
CMD_LIST_T cmdList;

extern void MQTT_Client_Init(void);
extern void MQTT_Client_Destroy(void);
extern void MQTT_Publish(char *buf, int len);

CMD_PARA_T cmdParaList[] = 
{
	{"id",        TYPE_TEXT, cmdList.id},
	{"log_level", TYPE_TEXT, cmdList.log_level},
	{"type",      TYPE_INT,  &cmdList.type},
};

void cbTimer1(sigval_t i)
{
	static uint8_t cnt = 0;
	char buf[] = {"hello"};

	MQTT_Publish(buf, strlen(buf));
	cnt++;
	if(cnt > 10)
	{
		MQTT_Client_Destroy();
		exit(-1);
	}
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
	
	addTimer(&gTimer1, 1, cbTimer1);
	startTimer(&gTimer1);
	
	MQTT_Client_Init();
	epollListenLoop(&gMainLoop);

	return 0;
}