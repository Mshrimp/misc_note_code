#include <stdio.h>
#include <string.h>

#define	TARGET_SPEED	100
#define	PID_CNT			100

typedef struct pid_info {
	float kp;
	float ki;
	float kd;
	float set_speed;
	float actual_speed;
	float err;
	float err_last1;
	float err_last2;
	float err_total;
} pid_info_s;

int pid_init(pid_info_s *pid)
{
	if (NULL == pid) {
		printf("error: pid is NULL\n");
		return -1;
	}

	pid->kp = 0.1;
	pid->ki = 0.2;
	pid->kd = 0.1;
	pid->set_speed = 0.0;
	pid->actual_speed = 0.0;
	pid->err = 0.0;
	pid->err_last1 = 0.0;
	pid->err_last2 = 0.0;
	pid->err_total = 0.0;

	return 0;
}

float pid_pos_calc(pid_info_s *pid, float speed)
{
	if ((NULL == pid) || (speed < 0)) {
		printf("error: pid is NULL\n");
		return -1;
	}

	pid->set_speed = speed;
	pid->err = pid->set_speed - pid->actual_speed;
	pid->err_total += pid->err;

	pid->actual_speed = pid->kp * pid->err + pid->ki * pid->err_total + pid->kd * (pid->err - pid->err_last1);

	pid->err_last2 = pid->err_last1;
	pid->err_last1 = pid->err;

	return pid->actual_speed;
}

float pid_inc_calc(pid_info_s *pid, float speed)
{
	float increat_speed = 0.0;

	if ((NULL == pid) || (speed < 0)) {
		printf("error: pid is NULL\n");
		return -1;
	}

	pid->set_speed = speed;
	pid->err = pid->set_speed - pid->actual_speed;

	increat_speed = pid->kp * (pid->err - pid->err_last1)+ pid->ki * pid->err + pid->kd * (pid->err - 2 * pid->err_last1 + pid->err_last2);

	pid->actual_speed += increat_speed;
	pid->err_last2 = pid->err_last1;
	pid->err_last1 = pid->err;

	return pid->actual_speed;
}

int main(void)
{
	int count = 0;
	float speed = 0.0;
	pid_info_s pid;

	memset(&pid, 0, sizeof(pid_info_s));
	pid_init(&pid);

	for (count = 0; count < PID_CNT; count++) {
#if 0
		speed = pid_pos_calc(&pid, TARGET_SPEED);
#else
		speed = pid_inc_calc(&pid, TARGET_SPEED);
#endif
		printf("count: %d, speed: %f\n", count, speed);
	}



	return 0;
}

