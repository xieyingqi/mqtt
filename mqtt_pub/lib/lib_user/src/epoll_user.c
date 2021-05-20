#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#include "epoll_user.h"
#include "log_printf.h"
/**
 * @brief 创建并初始化epoll监听文件
 * @param fdEvent 监听结构体
 * @return 0:成功 -1:失败
 */
int epollCreate(EP_LISTEN_T *fdEvent)
{
	memset(fdEvent, 0, sizeof(EP_LISTEN_T));
	fdEvent->epfd = epoll_create1(0);
	if (fdEvent->epfd < 0)
	{
		perror("create epoll");
		return -1;
	}
	logPrintf("create epfd:%d", fdEvent->epfd);

	return 0;	
}

/**
 * @brief 关闭epoll文件描述符
 * @param fdEvent 监听结构体
 */
void epollDesrory(EP_LISTEN_T *fdEvent)
{
	logPrintf("close epfd:%d", fdEvent->epfd);
	close(fdEvent->epfd);
}

/**
 * @brief 向epoll文件中添加监听文件
 * @param epfd epollCreate创建的返回值
 * @param listenFd 要添加的需要监听的文件
 * @return 0:成功 -1:失败
 */
int epollAddEvent(EP_LISTEN_T *fdEvent, int listenFd, cb_listenEvent cbFunc, void *cbArg)
{
	int ret;
	struct epoll_event event;

	event.data.fd = listenFd;
	event.events = EPOLLIN;

	ret = epoll_ctl(fdEvent->epfd, EPOLL_CTL_ADD, listenFd, &event);
	if (ret < 0)
	{
		logPrintf("epoll_ctl errno:%d", errno);
		return -1;
	}

	if (fdEvent->cnt < MAX_LISTEN)
	{
		fdEvent->event[fdEvent->cnt].fd = listenFd;
		fdEvent->event[fdEvent->cnt].cbFunc = cbFunc;
		fdEvent->event[fdEvent->cnt].cbArg = cbArg;
		fdEvent->cnt++;		

		logPrintf("add listenfd:%d to epfd:%d total:%d", listenFd, fdEvent->epfd, fdEvent->cnt);
	}
	else
	{
		logPrintf("event full");
		return -1;
	}
	
	return 0;
}

/**
 * @brief 开始监听
 * @param fdEvent 监听结构体
 */
void epollListenLoop(EP_LISTEN_T *fdEvent)
{
	int fds, i, j;
	struct epoll_event listenEvents[MAX_LISTEN] = {0};

	while (1)
	{
		fds = epoll_wait(fdEvent->epfd, listenEvents, MAX_LISTEN, -1);
		
		for (i = 0; i < fds; i++)
		{
			for (j = 0; j < MAX_LISTEN; j++)
			{
				if (listenEvents[i].data.fd == fdEvent->event[j].fd)
				{
					if (*fdEvent->event[j].cbFunc != NULL)
					{
						(*fdEvent->event[j].cbFunc)(fdEvent->event[j].fd, listenEvents[i], fdEvent->event[j].cbArg);
					}
					break;
				}
			}
		}
	}
}
