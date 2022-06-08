#ifndef FT_PWM_H
# define FT_PWM_H

int PWMExport (int pwmnum);
int PWMEnable (int pwmnum);
int PWMWritePeriod(int pwmnum, int value);
int PWMWriteDutyCycle(int pwmnum, int value);

#endif