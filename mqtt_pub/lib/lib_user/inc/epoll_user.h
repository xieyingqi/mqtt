#ifndef _EPOLL_USER_H_
#define _EPOLL_USER_H_

#include <sys/epoll.h>
#include <sys/errno.h>

#define MAX_LISTEN 10

typedef void (* cb_listenEvent)(int epfd, struct epoll_event ev, void *arg);
typedef struct
{
	int fd;				   //要监听的文件描述符
	cb_listenEvent cbFunc; //事件发生时的回调函数
	void *cbArg;           //回调函数的参数
} FD_EVENT_T;

typedef struct
{
	int epfd;  //epoll文件描述符
	int cnt;   //已添加的监听事件数量
	FD_EVENT_T event[MAX_LISTEN];
} EP_LISTEN_T;

extern int epollCreate(EP_LISTEN_T *fdEvent);
extern void epollDesrory(EP_LISTEN_T *fdEvent);
extern int epollAddEvent(EP_LISTEN_T *fdEvent, int listenFd, cb_listenEvent cbFunc, void *arg);
extern void epollListenLoop(EP_LISTEN_T *fdEvent);

#endif