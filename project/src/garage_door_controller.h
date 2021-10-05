#ifndef _GARAGE_DOOR_CONTROLLER_H
#define _GARAGE_DOOR_CONTROLLER_H


typedef enum
{
    BUTTON_PRESSED,
    FULLY_OPEN,
    FULLY_CLOSED,
    MOTOR_OVERCURRENT,
    IR_BREAK
} Event;

typedef enum
{
    UNINITIALIZED,
    CLOSED,
    RAISING,
    OPEN,
    LOWERING,
    STOPPED_RAISING,
    STOPPED_LOWERING
} State;

void gdc_init(void);
State gdc_process_event(Event e);

#endif // _GARAGE_DOOR_CONTROLLER_H
