#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#include "log_printf.h"
#include "unix_socket.h"

/**
 * @brief 创建本地套接字
 * @param sock 套接字结构体
 * @return 0:成功 -1:失败
 */
static int createUnixSocket(UNIX_SOCKET_T *sock, const char* path)
{
	/* 创建本地套接字 */
	sock->fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sock->fd < 0)
	{
		perror("socket error!");
		return -1;
	}
	logPrintf("create socket fd:%d", sock->fd);

	/* 结构体初始化 */
	bzero(&sock->addr, sizeof(sock->addr));
	sock->addr.sun_family = AF_UNIX;
	strcpy(sock->addr.sun_path, path);
	unlink(path);

	/* 绑定套接字 */
	sock->len = sizeof(sock->addr);
	if (bind(sock->fd, (struct sockaddr*)&(sock->addr), sock->len) < 0)
	{
		perror("bind error!\n");
		close(sock->fd);
		return -1;
	}

	return 0;
}

/**
 * @brief 删除本地套接字
 * @param sock 套接字结构体
 */
static void deleteUnixSocket(UNIX_SOCKET_T *sock)
{
	close(sock->fd);
}

/**
 * @brief 创建本地套接字监听(IPC通讯)
 * @param sock 套接字结构体
 * @param id 程序通讯id
 */
int creatIpc(EP_LISTEN_T *ep, SOCKET_IPC_T *ipc, const char* id)
{
	char path[50];

	strcpy(ipc->id, id);
	strcpy(ipc->recvMsg.id, ipc->id);
	ipc->ep = ep;

	sprintf(path, SOCKET_PATH, ipc->id);

	if (createUnixSocket(&ipc->sock, path) < 0)
	{
		deleteUnixSocket(&ipc->sock);
		return -1;
	}

	return 0;
}

/**
 * @brief 本地套接字发送
 * @param ipc 结构体
 * @param id 目标id
 * @param data 要发送的数据
 */
int sendIpc(SOCKET_IPC_T *ipc, const char* id, IPC_MSG_T data)
{
	UNIX_SOCKET_T sock;
	char path[50];

	sprintf(path, SOCKET_PATH, id);
	bzero(&sock.addr, sizeof(sock.addr));
	sock.addr.sun_family = AF_UNIX;
	strcpy(sock.addr.sun_path, path);
	sock.len = sizeof(sock.addr);

	sendto(ipc->sock.fd, data.buf, sizeof(data.buf), 0, (struct sockaddr *)&sock.addr, sock.len);
	logPrintf("ipc send to %s: %s", id, data.buf);

	return 0;
}

/**
 * @brief 本地套接字接收（同步）
 * @param ipc 结构体
 * @param id 目标id
 * @param data 接收到的数据指针
 * @param timeOutMs 超时时间
 */
int recvIpcSync(SOCKET_IPC_T *ipc, const char* id, IPC_MSG_T *data, int timeOutMs)
{
	int fds, i, j;
	size_t rd = 0;
	UNIX_SOCKET_T sock;
	struct epoll_event listenEvents[SOCKET_MAX] = {0};

	fds = epoll_wait(ipc->ep->epfd, listenEvents, SOCKET_MAX, timeOutMs);
	
	for (i = 0; i < fds; i++)
	{
		for (j = 0; j < SOCKET_MAX; j++)
		{
			if (listenEvents[i].data.fd == ipc->ep->event[j].fd)
			{
				sock.len = sizeof(sock.addr);
				rd = recvfrom(ipc->ep->event[j].fd, data->buf, sizeof(data->buf), 0, (struct sockaddr *)&sock.addr, &sock.len);
				logPrintf("ipc recv from %s: %s", id, data->buf);
				break;
			}
		}
	}

	return rd;
}

/**
 * @brief ipc可读事件回调函数
 * @param arg 参数
 */
static void cbRecvIpcMsg(int epfd, struct epoll_event ev, void *arg)
{
	UNIX_SOCKET_T sock;
	SOCKET_IPC_T *ipc = (SOCKET_IPC_T *)arg;

	sock.len = sizeof(sock.addr);
	ipc->recvMsg.cnt = recvfrom(ipc->ep->event->fd, ipc->recvMsg.buf, sizeof(ipc->recvMsg.buf), 0, (struct sockaddr *)&sock.addr, &sock.len);
	logPrintf("ipc recv from %s: %s", ipc->recvMsg.id, ipc->recvMsg.buf);

	ipc->cbArg = (void *)&ipc->recvMsg;
	if(*ipc->cbFunc != NULL)
	{
		(*ipc->cbFunc)(ipc->cbArg);
	}
}

/**
 * @brief 设置ipc回调函数
 * @param ipc 结构体
 * @param cbFunc 回调函数 NULL：同步模式  其他：异步模式
 */
int setIpcCallBack(SOCKET_IPC_T *ipc, cb_socket cbFunc)
{
	ipc->cbFunc = cbFunc;
	ipc->cbArg = (void *)ipc;

	if(epollAddEvent(ipc->ep, ipc->sock.fd, cbRecvIpcMsg, (void *)ipc) < 0)
	{
		return -1;
	}

	return 0;
}