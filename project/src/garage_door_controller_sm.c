#include "garage_door_controller_sm.h"

#include "winding_motor.h"
#include "light.h"

typedef void (*Handler)(void);

typedef struct 
{
    Handler handler;
    State   nextState;
} Transition;

/******************************************************************
  TRANSITION BEHAVIOURS

  These functions define what happens when a particular state
  transition occurs

*/

static void do_raising (void);      
static void do_open (void);      
static void do_lowering (void);      
static void do_closed (void);      
static void do_stopped_raising (void);      
static void do_stopped_lowering (void);      


const static Transition stateTable[NUM_STATES][NUM_EVENTS] =
{
/*                          BUTTON_PRESSED                  FULLY_OPEN                          FULLY_CLOSED                    MOTOR_OVERCURRENT                IR_BREAK
    ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
[CLOSED][BUTTON_PRESSED]           = { .handler = do_raising,          .nextState = RAISING          },
[RAISING][BUTTON_PRESSED]          = { .handler = do_stopped_raising,  .nextState = STOPPED_RAISING  },    
[RAISING][FULLY_OPEN]              = { .handler = do_open,             .nextState = OPEN             },             
[RAISING][MOTOR_OVERCURRENT]       = { .handler = do_stopped_raising,  .nextState = STOPPED_RAISING  }, 
[OPEN][BUTTON_PRESSED]             = { .handler = do_lowering,         .nextState = LOWERING         },     
[LOWERING][BUTTON_PRESSED]         = { .handler = do_stopped_lowering, .nextState = STOPPED_LOWERING },   
[LOWERING][FULLY_CLOSED]           = { .handler = do_closed,           .nextState = CLOSED           },             
[LOWERING][MOTOR_OVERCURRENT]      = { .handler = do_stopped_lowering, .nextState = STOPPED_LOWERING },          
[LOWERING][IR_BREAK]               = { .handler = do_stopped_lowering, .nextState = STOPPED_LOWERING },
[STOPPED_RAISING][BUTTON_PRESSED]  = { .handler = do_lowering,         .nextState = LOWERING         },           
[STOPPED_LOWERING][BUTTON_PRESSED] = { .handler = do_raising,          .nextState = RAISING          }
};


static State current_state = UNINITIALIZED;

void gdc_init(void)
{
    current_state = CLOSED;
}

State gdc_process_event(Event event)
{
    if(stateTable[current_state][event].handler != UNINITIALIZED) {
        stateTable[current_state][event].handler();
        current_state = stateTable[current_state][event].nextState;        
    }
    return current_state;
}


void do_raising (void)
{
    light_on();
    motor_on(CLOCKWISE);
}

void do_open(void)
{
    motor_off();
    light_off();
}

void do_lowering(void)
{
    light_on();
    motor_on(ANTI_CLOCKWISE);
}

void do_closed(void)
{
    motor_off();
    light_off();
}

void do_stopped_raising (void)
{
    motor_off();
    light_off();
}

void do_stopped_lowering (void)
{
    motor_off();
    light_off();
}

