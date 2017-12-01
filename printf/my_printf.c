#include <stdio.h>
#include <stdarg.h>


#define ECHO_COLOR_NONE			"\033[0;0m"
#define ECHO_COLOR_GREEN		"\033[0;32m"

//#define debug(fmt, args...)		printf("Debug: " fmt "(file: %s, func: %s, line: %d)\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__);
#define debug(fmt, args...)		printf(ECHO_COLOR_GREEN"Debug: " fmt "(file: %s, func: %s, line: %d)\n"ECHO_COLOR_NONE, ##args, __FILE__, __func__, __LINE__);


#define debug_print(message, ...)	debug_msg(message, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
void debug_type(char *fmt, char *file, const char *func, int line, va_list argp)
{
	char buffer[128] = { 0 };
	
	vsnprintf(buffer, sizeof(buffer), fmt, argp);
	printf(ECHO_COLOR_GREEN"Debug: %s(file: %s, func: %s, line: %d)\n"ECHO_COLOR_NONE, buffer, file, func, line);
}


void debug_msg(char *fmt, char *file, const char *func, int line, ...)
{
	va_list	arg_list;
	va_start(arg_list, line);
	debug_type(fmt, file, func, line, arg_list);
	va_end(arg_list);
}



int main(void)
{
	int data = 10;

	debug("This is a test, data = %d", data);

	debug_print("This is a test, data = %d", data);
	printf("OK\n");

	return 0;
}

