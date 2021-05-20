#ifndef _TIMER_USER_H_
#define _TIMER_USER_H_

#include <sys/time.h>
#include <time.h>
#include <signal.h>

enum TIMER_MODE
{
	eTIM_ONCE,
	eTIM_EVRY,
};

typedef void (*cb_timerFunc) (sigval_t);

typedef struct
{
	timer_t id;
	int s_gap;
	int ms_gap;
	cb_timerFunc cbFunc;

	struct sigevent ev;
	struct itimerspec ts;
} TIMER_STRU_T;

extern int addTimer(TIMER_STRU_T *timer, int gap, cb_timerFunc cbFunc);
extern int addTimerMs(TIMER_STRU_T *timer, int gap, cb_timerFunc cbFunc);
extern int startTimer(TIMER_STRU_T *timer);
extern int delTimer(TIMER_STRU_T *timer);

#endif