#ifndef FT_GPIO_H
# define FT_GPIO_H

#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1
#define POUT 23
#define PIN 24
#define SENSOR 25
#define VALUE_MAX 256
#define DIRECTION_MAX 45
#define DISTANCE 10

int GPIOExport(int pin);
int GPIOUnexport(int pin);
int GPIODirection(int pin, int dir);
int GPIORead(int pin);
int GPIOWrite(int pin, int value);
int export_sensor_gpio();
int direction_sensor_gpio();
int unexport_sensor_gpio();

#endif
