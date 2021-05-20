#ifndef _UNIX_SOCKET_H_
#define _UNIX_SOCKET_H_

#include <sys/un.h>
#include <sys/socket.h>

#include "epoll_user.h"
#include "cmd_para_parse.h"

#define SOCKET_PATH "/tmp/%s.unix"
#define SOCKET_MAX 10

typedef struct
{
	int fd;
	socklen_t len;
	struct sockaddr_un addr;
} UNIX_SOCKET_T;

#define MAX_MSG_BUF 1024
typedef struct
{
	char id[MAX_CMD_LEN];
	int cnt;
	char buf[MAX_MSG_BUF];
} IPC_MSG_T;

typedef void (*cb_socket)(IPC_MSG_T *ipc);

typedef struct
{
	char id[MAX_CMD_LEN];

	UNIX_SOCKET_T sock;
	EP_LISTEN_T *ep;

	IPC_MSG_T recvMsg;

	cb_socket cbFunc;
	void *cbArg;
} SOCKET_IPC_T;

extern int creatIpc(EP_LISTEN_T *ep, SOCKET_IPC_T *ipc, const char* id);
extern int setIpcCallBack(SOCKET_IPC_T *ipc, cb_socket cbFunc);
extern int sendIpc(SOCKET_IPC_T *ipc, const char* id, IPC_MSG_T data);
extern int recvIpcSync(SOCKET_IPC_T *ipc, const char* id, IPC_MSG_T *data, int timeOutMs);

#endif