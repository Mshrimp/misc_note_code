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

typedef int (*sym_func_t)(int);

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

	//cnt = sizeof(symbol_tbl) / sizeof(symbol_tbl[0]);
	cnt = symbol_cnt;
	//printf("%s, cnt: %d\n", __func__, cnt);

	for (i = 0; i < cnt; i++) {
		if (!memcmp(cmd, symbol_tbl[i].name, strlen(cmd))) {
			//printf("%s, i: %d, %s\n", __func__, i, symbol_tbl[i].name);
			break;
		}
	}

	if (i == cnt) {
		printf("%s, i: %d, cnt: %d, no found func in symbol_tbl!\n", __func__, i, cnt);
		return -1;
	}

	symbol->name = symbol_tbl[i].name;
	symbol->addr = symbol_tbl[i].addr;

	return 0;
}

int run_symbol_func(symbol_t *symbol)
{
	sym_func_t sym_func;

	//printf("%s, %d\n", __func__, __LINE__);
	sym_func = (sym_func_t *)symbol->addr;
	//printf("%s, %d\n", __func__, __LINE__);

	sym_func(1);
	//printf("%s, %d\n", __func__, __LINE__);

	return 0;
}

int cmd_process(unsigned char *cmd)
{
	symbol_t symbol;

	memset(&symbol, 0, sizeof(symbol_t));
	check_symbol(cmd, &symbol);

	//printf("%s, %d\n", __func__, __LINE__);
	run_symbol_func(&symbol);

	return 0;
}

#define CMD_LEN 		(64)
unsigned char cmd[CMD_LEN] = {0};
int main(void)
{
	printf("Hello world!\n");

	//check_symbol(cmd);

	while (1) {
		printf("Debug->");
		memset(cmd, 0, CMD_LEN);
		scanf("%s", cmd);
		printf("cmd: %s\n", cmd);
		if (!(memcmp("quit", cmd, 4)) || !(memcmp("exit", cmd, 4))) {
			printf("quit debug process!\n");
			break;
		}
		if (!(memcmp(cmd, "\n", 2))) {
			continue;
		}
		cmd_process(cmd);
	}

	return 0;
}
