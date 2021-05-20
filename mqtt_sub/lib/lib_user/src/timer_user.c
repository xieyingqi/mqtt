#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#include "log_printf.h"
#include "timer_user.h"

/**
 * @brief 添加定时器s
 * @param timer  定时器结构体
 * @param gap    周期
 * @param cbFunc 回调函数
 * @return 0:成功 -1:失败
 */
int addTimer(TIMER_STRU_T *timer, int gap, cb_timerFunc cbFunc)
{
	timer->s_gap = gap;
	timer->ms_gap = 0;
	timer->cbFunc = cbFunc;

	timer->ev.sigev_value.sival_ptr = &timer->id;
	timer->ev.sigev_notify = SIGEV_THREAD;
	timer->ev.sigev_notify_function = timer->cbFunc;

	if(timer_create(CLOCK_REALTIME, &timer->ev, &timer->id) < 0)
	{
		perror("timer create error!\n");
		return -1;
	}

	return 0;
}

/**
 * @brief 添加定时器ms
 * @param timer  定时器结构体
 * @param gap    周期
 * @param cbFunc 回调函数
 * @return 0:成功 -1:失败
 */
int addTimerMs(TIMER_STRU_T *timer, int gap, cb_timerFunc cbFunc)
{
	timer->s_gap = 0;
	timer->ms_gap = gap;
	timer->cbFunc = cbFunc;

	timer->ev.sigev_value.sival_ptr = &timer->id;
	timer->ev.sigev_notify = SIGEV_THREAD;
	timer->ev.sigev_notify_function = timer->cbFunc;

	if(timer_create(CLOCK_REALTIME, &timer->ev, &timer->id) < 0)
	{
		perror("timer create error!\n");
		return -1;
	}

	return 0;
}

/**
 * @brief 启动定时器
 * @param timer 定时器结构体
 * @return 0:成功 -1:失败
 */
int startTimer(TIMER_STRU_T *timer)
{
	timer->ts.it_interval.tv_sec = timer->s_gap;
	timer->ts.it_interval.tv_nsec = timer->ms_gap * 1000 * 1000;
	timer->ts.it_value.tv_sec = timer->s_gap;
	timer->ts.it_value.tv_nsec = timer->ms_gap * 1000 * 1000;

	if(timer_settime(timer->id, 0, &timer->ts, NULL) < 0)
	{
		perror("timer start error!\n");
		return -1;
	}
	
	return 0;
}

/**
 * @brief 删除定时器
 * @param timer 定时器结构体
 * @return 0:成功 -1:失败
 */
int delTimer(TIMER_STRU_T *timer)
{
	if(timer_delete(timer->id) < 0)
	{
		perror("timer del error!\n");
		return -1;
	}
	
	return 0;
}