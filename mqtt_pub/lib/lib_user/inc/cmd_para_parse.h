#ifndef _CMD_PARA_PARSE_H_
#define _CMD_PARA_PARSE_H_

enum PARA_TYPE_E
{
	TYPE_INT,
	TYPE_TEXT,
};

#define MAX_CMD_LEN 50
typedef struct
{
	char id[MAX_CMD_LEN];
	char log_level[MAX_CMD_LEN];
	int type;
} CMD_LIST_T;

typedef struct
{
	const char *name;      //参数名称
	enum PARA_TYPE_E type; //参数类型
	void *value;           //参数值
} CMD_PARA_T;

extern void parseCmdPara(int argc, char *argv[], CMD_PARA_T *cmdList, int cmdCnt);

#endif