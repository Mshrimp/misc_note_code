#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct symbol {
	unsigned char *name;
	unsigned char *addr;
} symbol_t;

extern symbol_t symbol_tbl[];
extern int symbol_cnt;

typedef int (*sym_func_t)(int, int, int, int);

#define DEBUG_ARGC_MAX	(4)
int args[DEBUG_ARGC_MAX] = {0};

int symbol_parse(unsigned char *str, symbol_t *sym)
{
	unsigned char addr[20] = {0};
	unsigned char status;
	unsigned char name[32] = {0};
	char gap = ' ';
	unsigned char *p = NULL;
	int i = 0;

	p = strtok(str, &gap);
	while (p != NULL) {
		printf("p: %s\n", p);
		if (0 == i) {
			memcpy(addr, p, strlen(p) + 1);
		} else if (1 == i) {
			status = *p;
		} else if (2 == i) {
			memcpy(name, p, strlen(p) + 1);
		}

		i++;
		p = strtok(NULL, &gap);
	}
	printf("addr: %s\n", addr);
	printf("name: %s\n", name);

	sym->name = (unsigned char *)name;
	sym->addr = (unsigned char *)strtoull(addr, NULL, 16);
	//memcpy(sym->name, name, strlen(name) + 1);

	printf("sym->addr: %p\n", sym->addr);
	printf("sym->name: %s\n", sym->name);
	printf("OK!\n");

	return 0;
}

#define SYMBOL_LEN		(64)
#define SYMBOL_FILE		("debug.sym")
#if 0
int check_symbol(unsigned char *cmd, symbol_t *symbol)
{
	unsigned char symbol_line[SYMBOL_LEN] = {0};
	FILE *fp = NULL;

	fp = fopen(SYMBOL_FILE, "r");
	if (!fp) {
		perror("fopen");
		return -1;
	}

	while (fgets(symbol_line, SYMBOL_LEN, fp)) {
		if (!strstr(symbol_line, "test_")) {
			continue;
		}
		printf("%s", symbol_line);

		if (strstr(symbol_line, cmd)) {
			break;
		}
	}

	symbol_parse(symbol_line, symbol);
	printf("%s, %p\n", __func__, symbol_parse);

	printf("%s, %d\n", __func__, __LINE__);

	return 0;
}
#endif

int check_symbol(unsigned char *cmd, symbol_t *symbol)
{
	int cnt = 0;
	int i = 0;

	cnt = symbol_cnt;
	for (i = 0; i < cnt; i++) {
		if (!memcmp(cmd, symbol_tbl[i].name, strlen(symbol_tbl[i].name))) {
			//printf("%s, i: %d, %s\n", __func__, i, symbol_tbl[i].name);
			break;
		}
	}

	if (i == cnt) {
		//printf("%s, i: %d, cnt: %d, no found func in symbol_tbl!\n", __func__, i, cnt);
		return -1;
	}

	symbol->name = symbol_tbl[i].name;
	symbol->addr = symbol_tbl[i].addr;
	printf("function: %s\n", symbol->name);

	return 0;
}

int check_args(unsigned char *cmd, int *args)
{
	unsigned char arg[16] = {0};
	unsigned char *p_prev = NULL;
	unsigned char *p_next = NULL;
	int len = 0;
	int cnt = 0;
	int end = 0;
	int i = 0;

	p_prev = cmd;

	while (*p_prev == ' ') {
		p_prev++;
	}

	while ((!end) && (*p_prev != '\0')) {
		while (*p_prev == ' ') {
			p_prev++;
		}

		p_next = strchr(p_prev, ' ');

		memset(arg, 0, 16);
		if (p_next) {
			len = p_next - p_prev;
			memcpy(arg, p_prev, len);
		} else {
			len = strlen(p_prev);
			memcpy(arg, p_prev, len);
			end = 1;
		}

		//printf("%s, arg: %s\n", __func__, arg);

		args[cnt] = strtol(arg, NULL, 10);
		//printf("%s, args[%d]: %d\n", __func__, cnt, args[cnt]);

		cnt++;

		p_prev = p_next;
		p_next = NULL;
	}

	
	for (i = 0; i < cnt; i++) {
		printf("args[%d]: %d\n", i, args[i]);
	}

	printf("arg cnt: %d\n", cnt);

	return 0;
}

int run_symbol_func(symbol_t *symbol, int *args)
{
	sym_func_t sym_func;
	int ret = -1;

	//printf("%s, %d\n", __func__, __LINE__);
	//sym_func = (sym_func_t *)symbol->addr;
	ret = ((sym_func_t)(symbol->addr))(args[0], args[1], args[2], args[3]);
	//printf("%s, %d\n", __func__, __LINE__);
	if (ret < 0) {
		printf("%s run failed, ret: %d\n", symbol->name, ret);
	} else {
		printf("Done, return: %d\n", ret);
	}

	//sym_func(1);
	//printf("%s, %d\n", __func__, __LINE__);

	return 0;
}

int cmd_process(unsigned char *cmd)
{
	symbol_t symbol;
	int ret = -1;

	memset(&symbol, 0, sizeof(symbol_t));
	ret = check_symbol(cmd, &symbol);
	if (ret < 0) {
		printf("%s, check_symbol failed, ret: %d\n", __func__, ret);
		return ret;
	}
	cmd += strlen(symbol.name);
	check_args(cmd, args);

	run_symbol_func(&symbol, args);

	return 0;
}

#define CMD_LEN 		(64)
unsigned char cmd[CMD_LEN] = {0};
int main(void)
{
	unsigned char *p = NULL;
	int ret = -1;

	while (1) {
		printf("Debug->");
		memset(cmd, 0, CMD_LEN);
		scanf("%[^\n]", cmd);
		//printf("%s, cmd: %s\n", __func__, cmd);
		getchar();
		//fflush(stdin);
		if (!(memcmp("quit", cmd, 4)) || !(memcmp("exit", cmd, 4))) {
			printf("quit debug process!\n");
			break;
		}
		if (cmd[0] == '\0') {
			continue;
		}

		if (!(memcmp(cmd, "\n", 1))) {
			continue;
		}

		p = cmd;
		while ((*p != '\0') && ((*p < 'A') || (*p > 'z'))) {
			p++;
		}
		if (*p =='\0') {
			continue;
		}

		ret = cmd_process(p);
		if (ret < 0) {
			printf("%s, cmd_process failed, ret: %d\n", __func__, ret);
			continue;
		}
	}

	return 0;
}

