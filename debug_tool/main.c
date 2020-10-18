#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int test_1(int ind)
{
	printf("%s, ind: %d\n", __func__, ind);

	return 0;
}

int test_2(int ind)
{
	printf("%s, ind: %d\n", __func__, ind);

	return 0;
}

typedef struct symbol {
	unsigned long long addr;
	unsigned char status;
	unsigned char name[32];
} symbol_t;

typedef int (*sym_func_t)(void);

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
	printf("status: %c\n", status);
	printf("name: %s\n", name);

	sym->addr = strtoull(addr, NULL, 16);
	sym->status = status;
	memcpy(sym->name, name, strlen(name) + 1);

	printf("sym->addr: 0x%016llx\n", sym->addr);
	printf("sym->status: %c\n", sym->status);
	printf("sym->name: %s\n", sym->name);
	printf("OK!\n");

	return 0;
}

#define SYMBOL_LEN		(64)
#define SYMBOL_FILE		("main.sym")
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

int run_symbol_func(symbol_t *symbol)
{
	sym_func_t sym_func;

	printf("%s, %d\n", __func__, __LINE__);
	sym_func = (sym_func_t *)symbol->addr;
	printf("%s, %d\n", __func__, __LINE__);

	sym_func();
	printf("%s, %d\n", __func__, __LINE__);

	return 0;
}

int cmd_process(unsigned char *cmd)
{
	symbol_t symbol;

	memset(&symbol, 0, sizeof(symbol_t));
	check_symbol(cmd, &symbol);

	printf("%s, %d\n", __func__, __LINE__);
	run_symbol_func(&symbol);

	return 0;
}

#define CMD_LEN 		(64)
unsigned char cmd[CMD_LEN] = {0};
int main(void)
{
	printf("Hello world!\n");

	test_1(1);
	test_2(2);

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
		cmd_process(cmd);
	}

	return 0;
}
