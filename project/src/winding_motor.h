#ifndef _WINDING_MOTOR_H
#define _WINDING_MOTOR_H

typedef enum { CLOCKWISE, ANTI_CLOCKWISE } Direction;

void motor_on(Direction dir);
void motor_off(void);


#endif // _WINDING_MOTOR_H
