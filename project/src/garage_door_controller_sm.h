#ifndef _GARAGE_DOOR_CONTROLLER_SM_H
#define _GARAGE_DOOR_CONTROLLER_SM_H

typedef enum
{
    BUTTON_PRESSED,
    FULLY_OPEN,
    FULLY_CLOSED,
    MOTOR_OVERCURRENT,
    IR_BREAK,
    NUM_EVENTS
} Event;

typedef enum
{
    UNINITIALIZED,
    CLOSED,
    RAISING,
    OPEN,
    LOWERING,
    STOPPED_RAISING,
    STOPPED_LOWERING,
    NUM_STATES
} State;

void gdc_init(void);
State gdc_process_event(Event e);

#endif // _GARAGE_DOOR_CONTROLLER_SM_H
