#include "garage_door_controller.h"
#include "winding_motor.h"
#include "light.h"

static State current_state = UNINITIALIZED;

void gdc_init(void)
{
    current_state = CLOSED;
}

State gdc_process_event(Event e)
{
    switch(e) {
        case BUTTON_PRESSED:
            switch(current_state) {
                case CLOSED:
                case STOPPED_LOWERING:
                    current_state = RAISING;
                    light_on();
                    motor_on(CLOCKWISE);
                    break;                    
                case OPEN:
                case STOPPED_RAISING:
                    current_state = LOWERING;
                    light_on();
                    motor_on(ANTI_CLOCKWISE);
                    break;    
                case RAISING:
                    current_state = STOPPED_RAISING;
                    motor_off();
                    light_off();
                    break; 
                case LOWERING:
                    current_state = STOPPED_LOWERING;
                    motor_off();
                    light_off();
                    break; 
                // case UNINITIALIZED:
                default:
                    break;
            }
            break;
        case FULLY_OPEN:
            if(current_state == RAISING) {
                current_state = OPEN;
                motor_off();
                light_off();
            }
            break;
        case FULLY_CLOSED:
            if(current_state == LOWERING) {
                current_state = CLOSED;
                motor_off();
                light_off();
            } 
            break;
        case MOTOR_OVERCURRENT:
            switch(current_state) {
                case RAISING:
                    current_state = STOPPED_RAISING;
                    motor_off();
                    light_off();
                    break; 
                case LOWERING:
                    current_state = STOPPED_LOWERING;
                    motor_off();
                    light_off();
                    break; 
                // case CLOSED:
                // case STOPPED_LOWERING:                  
                // case OPEN:
                // case STOPPED_RAISING: 
                // case UNINITIALIZED:
                default:
                    break;
            }
            break;
        case IR_BREAK:
            if(current_state == LOWERING) {
                current_state = STOPPED_LOWERING;
                motor_off();
                light_off();
            }
    }
    return current_state;
}
