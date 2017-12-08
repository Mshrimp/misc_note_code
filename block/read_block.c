#include <stdio.h>
#include <unistd.h>



int main(void)
{
	char buf[10] = { 0 };

	int len = 0;

	while (1) {
		len = read(0, buf, 10);
		printf("read len = %d, buf = %s\n", len, buf);
		write(1, buf, len);
	}


	return 0;
}

