#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>
#include <syslog.h>

#include "log_printf.h"

static char appName[128];

/* 打印级别相关结构 */
typedef enum
{
	LOG_NONE,
	LOG_DBG
} LOG_LEVEL_T;

typedef struct
{
	char *str;
	LOG_LEVEL_T en;
} LOG_LEVEL_MAP;

static LOG_LEVEL_MAP log_level_map[] = 
{
	{"none", LOG_NONE},
	{"debug", LOG_DBG}
};

static LOG_LEVEL_T slogLevel = LOG_NONE;

/**
 * @brief 获取系统时间
 */
static char *getSysTime(void)
{
	static char buf[50] = {0};
	char ms[10] = {0};
	struct tm time;
	struct timeval tv;

	if (gettimeofday(&tv, NULL) < 0)
	{
		perror("gettimeofday()");
		return "error time";
	}

	memset(buf, 0, sizeof(buf));
	memset(ms, 0, sizeof(ms));
	memset(&time, 0, sizeof(time));

	localtime_r(&tv.tv_sec, &time);
	strftime(buf, sizeof(buf), "%Y.%m.%d %H:%M:%S", &time);
	sprintf(ms, ".%03d", (int)(tv.tv_usec/1000));
	strcat(buf, ms);

	return buf;
}

/**
 * @brief 获取应用程序名称
 */
static char *getAppName(void)
{
	char path[1024] = {0};

	if(readlink("/proc/self/exe", path,1024) <= 0)
	{
		perror("readlink");
		return "unknown app";
	}
 
	return strrchr(path, '/');
}

/**
 * @brief 设置打印级别
 * @param log_level 打印级别
 */
void setLogLevel(const char *log_level)
{
	int i;

	for (i = 0; i < (sizeof(log_level_map) / sizeof(LOG_LEVEL_MAP)); i++)
	{
		if (strcmp(log_level_map[i].str, log_level) == 0)
		{
			slogLevel = log_level_map[i].en;
			strcpy(appName, getAppName());
			openlog(appName, LOG_PID, LOG_USER);
			break;
		}
	}
}

/**
 * @brief 打印信息
 */
void logPrintf(const char *format, ...)
{
	char buf[4096] = {0};
	va_list args;

	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

	switch (slogLevel)
	{
	case LOG_DBG:
		printf("##[log][%s][%s]: ===> %s\n", appName, getSysTime(), buf);
		syslog(LOG_USER, "%s", buf);
		break;
	
	default:
		break;
	}
}
