#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

char msg_retry[] = "Retry!\n";

char msg_timeout[] = "Timeout!\n";

int main(void)
{
	char buf[10] = { 0 };
	int fd = -1;
	int len = 0;
	int i = 0;

	fd = open("/dev/tty", O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		perror("open failed!\n");
		return -1;
	}

	for (i = 0; i < 5; i++) {
		len = read(fd, buf, 10);
		if (len < 0) {
			if (errno == EAGAIN) {
				printf("No data: len = %d, %s\n", len, msg_retry);
				write(fd, msg_retry, strlen(msg_retry));
				sleep(1);
			} else {
				perror("read failed!\n");
				exit(1);
			}
		} else {
			printf("read len = %d, buf = %s\n", len, buf);
			write(1, buf, len);
			break;
		}

	}

	if (i == 5) {
		printf("out: read %s", msg_timeout);	
	} else {
		printf("out: read i = %d, len = %d, buf = %s\n", i, len, buf);
		//write(1, buf, len);
	}

	return 0;
}

