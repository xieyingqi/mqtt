#ifndef _LOG_PRINTF_H_
#define _LOG_PRINTF_H_

extern void setLogLevel(const char *log_level);
extern void logPrintf(const char *format, ...);

#endif