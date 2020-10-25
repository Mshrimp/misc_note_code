#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#define SYMBOL_ELF_FILE	"main.sym"
#define SYMBOL_C_SOURCE	"symbol.c"

#define BUF_SIZE		(256)
#define CMD_SIZE		(32)
#define SYMBOL_SIZE		(128)

unsigned char buf[BUF_SIZE] = {0};
unsigned char cmd[CMD_SIZE] = {0};
unsigned char symbol_buf[SYMBOL_SIZE] = {0};

unsigned char *in_file = NULL;
unsigned char *out_file = NULL;
FILE *in_fp = NULL;
FILE *out_fp = NULL;

int symbol_write_to_file(unsigned char *buf, int size)
{
	int ret = -1;

	if (!out_fp) {
		//printf("%s, out_fp is null, open!\n", __func__);
		out_fp = fopen(out_file, "w");
		if (!out_fp) {
			printf("%s, out_fp is null!\n", __func__);
			return -1;
		}
	}

	ret = fwrite(buf, size, 1, out_fp);
	if (ret < 0) {
		printf("%s, fwrite failed, ret: %d\n", __func__, ret);
		return ret;
	}
	//printf("%s, ret: %d\n", __func__, ret);

	//fclose(out_fp);

	return 0;
}

int format_symbol_to_file(unsigned char *str)
{
	int len = 0;
	int ret = -1;

	memset(symbol_buf, 0, SYMBOL_SIZE);
	sprintf(symbol_buf, "%s", str);
	len = strlen(symbol_buf);
	//printf("%s, symbol_buf: %s, len: %d\n", __func__, symbol_buf, len);
	ret = symbol_write_to_file(symbol_buf, len);
	if (ret < 0) {
		printf("%s, symbol_write_to_file failed, ret: %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

int format_struct(void)
{
	format_symbol_to_file("\ntypedef struct {\n");
	format_symbol_to_file("    unsigned char *name;\n");
	format_symbol_to_file("    unsigned char *addr;\n");
	format_symbol_to_file("} symbol_t;\n\n");

	return 0;
}

int get_extern_func_from_buf(unsigned char *buf, int len)
{
	unsigned char *p = NULL;
	int func_len = 0;
	int i = 0;
	int ret = -1;

	p = strrchr(buf, ' ');
	if (!p) {
		printf("%s, strrchr failed!\n", __func__);
		return -1;
	}
	p++;
	func_len = strlen(p);
	//printf("%s, p: %s, len: %d\n", __func__, p, func_len);
	*(p + func_len - 1) = '\0';
	*(p + func_len) = '\n';
	//printf("%s, p: %s, len: %d\n", __func__, p, func_len);

	memset(cmd, 0, CMD_SIZE);
	memcpy(cmd, p, func_len);
	//printf("%s, cmd: %s\n", __func__, cmd);

	memset(symbol_buf, 0, SYMBOL_SIZE);
	sprintf(symbol_buf, "extern int %s(void);\n", cmd);
	func_len = strlen(symbol_buf);
	//printf("%s, symbol_buf: %s, len: %d\n", __func__, symbol_buf, func_len);

	ret = symbol_write_to_file(symbol_buf, func_len);
	if (ret < 0) {
		printf("%s, symbol_write_to_file failed, ret: %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

int get_struct_func_from_buf(unsigned char *buf, int len)
{
	unsigned char *p = NULL;
	int func_len = 0;
	int i = 0;
	int ret = -1;

	p = strrchr(buf, ' ');
	if (!p) {
		printf("%s, strrchr failed!\n", __func__);
		return -1;
	}
	p++;
	func_len = strlen(p);
	//printf("%s, p: %s, len: %d\n", __func__, p, func_len);
	*(p + func_len - 1) = '\0';
	*(p + func_len) = '\n';
	//printf("%s, p: %s, len: %d\n", __func__, p, func_len);

	memset(cmd, 0, CMD_SIZE);
	memcpy(cmd, p, func_len);
	//printf("%s, cmd: %s\n", __func__, cmd);

	memset(symbol_buf, 0, SYMBOL_SIZE);
	sprintf(symbol_buf, "    {\"%s\", &(char *)%s()},\n", cmd, cmd);
	func_len = strlen(symbol_buf);
	//printf("%s, symbol_buf: %s, len: %d\n", __func__, symbol_buf, func_len);

	ret = symbol_write_to_file(symbol_buf, func_len);
	if (ret < 0) {
		printf("%s, symbol_write_to_file failed, ret: %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

int symbol_extern_proc(unsigned char *p, int len)
{
	int ret = -1;

	//printf("p: %s\n", p);

	ret = get_extern_func_from_buf(p, len);
	if (ret < 0) {
		printf("%s, get_struct_func_from_buf failed, ret: %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

int symbol_struct_proc(unsigned char *p, int len)
{
	int ret = -1;

	//printf("p: %s\n", p);

	ret = get_struct_func_from_buf(p, len);
	if (ret < 0) {
		printf("%s, get_struct_func_from_buf failed, ret: %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

int symbol_extern_func_proc(void)
{
	unsigned char *p = NULL;
	int len = 0;
	int ret = -1;

	if (!in_fp) {
		in_fp = fopen(in_file, "r");
		if (!in_fp) {
			printf("%s, in_fp is null!\n", __func__);
			return -1;
		}
	}

	ret = fseek(in_fp, 0, SEEK_SET);
	if (ret < 0) {
		printf("%s, fseek failed, ret: %d\n", __func__, ret);
		return -2;
	}

	while (!feof(in_fp)) {
		memset(buf, 0, BUF_SIZE);
		p = fgets(buf, BUF_SIZE, in_fp);
		if (!p) {
			//printf("%s, fgets failed!\n", __func__);
			break;
		}

		len = strlen(p) + 1;
		//printf("%s, len: %d\n", __func__, len);

		ret = symbol_extern_proc(p, len);
		if (ret < 0) {
			printf("%s, symbol_proc failed, ret: %d\n", __func__, ret);
			return -3;
		}
	}

	fclose(in_fp);
	in_fp = NULL;

	return 0;
}

int symbol_struct_func_proc(void)
{
	unsigned char *p = NULL;
	int len = 0;
	int ret = -1;

	if (!in_fp) {
		in_fp = fopen(in_file, "r");
		if (!in_fp) {
			printf("%s, in_fp is null!\n", __func__);
			return -1;
		}
	}

	ret = fseek(in_fp, 0, SEEK_SET);
	if (ret < 0) {
		printf("%s, fseek failed, ret: %d\n", __func__, ret);
		printf("%s, errno: %d, %s\n", __func__, errno, strerror(errno));
		return -2;
	}

	format_symbol_to_file("symbol_t symbol_tbl[] = {\n");

	while (!feof(in_fp)) {
		memset(buf, 0, BUF_SIZE);
		p = fgets(buf, BUF_SIZE, in_fp);
		if (!p) {
			//printf("%s, fgets failed!\n", __func__);
			break;
		}

		len = strlen(p) + 1;
		//printf("%s, len: %d\n", __func__, len);

		ret = symbol_struct_proc(p, len);
		if (ret < 0) {
			printf("%s, symbol_proc failed, ret: %d\n", __func__, ret);
			return -2;
		}
	}

	format_symbol_to_file("};\n");

	fclose(in_fp);
	in_fp = NULL;

	return 0;
}

int main(int argc, char *argv[])
{
	unsigned char *p = NULL;
	int len = 0;
	int i = 0;
	int ret = -1;

	switch (argc) {
	case 1:
		in_file = SYMBOL_ELF_FILE;
		out_file = SYMBOL_C_SOURCE;
		break;
	case 2:
		in_file = argv[1];
		out_file = SYMBOL_C_SOURCE;
		break;
	case 3:
		in_file = argv[1];
		out_file = argv[2];
		break;
	default:
		printf("%s, argc: %d!\n", __func__, argc);
		break;
	}
	printf("%s, in_file: %s, out_file: %s\n", __func__, in_file, out_file);

	ret = symbol_extern_func_proc();
	if (ret < 0) {
		printf("%s, symbol_extern_func_proc failed, ret: %d\n", __func__, ret);
		return -1;
	}

	ret = format_struct();
	if (ret < 0) {
		printf("%s, format_struct failed, ret: %d\n", __func__, ret);
		return -2;
	}

	ret = symbol_struct_func_proc();
	if (ret < 0) {
		printf("%s, symbol_struct_func_proc failed, ret: %d\n", __func__, ret);
		return -3;
	}

	fclose(out_fp);

	return 0;
}

