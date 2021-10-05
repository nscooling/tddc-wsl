#include "unity.h"
#include "garage_door_controller.h"

#include "mock_light.h"
#include "mock_winding_motor.h"

void setUp(void)
{
    gdc_init();
}

void tearDown(void)
{
}

static State actual_state;

void test_garage_door_controller_Closed_to_Open(void)
{
    light_on_Expect();
    motor_on_Expect(CLOCKWISE);

    actual_state = gdc_process_event(BUTTON_PRESSED);
    TEST_ASSERT_EQUAL(RAISING, actual_state);
    
    motor_off_Expect();
    light_off_Expect();

    actual_state = gdc_process_event(FULLY_OPEN);
    TEST_ASSERT_EQUAL(OPEN, actual_state);
}

void test_garage_door_controller_Open_to_Closed(void)
{
    test_garage_door_controller_Closed_to_Open();

    light_on_Expect();
    motor_on_Expect(ANTI_CLOCKWISE);

    actual_state = gdc_process_event(BUTTON_PRESSED);
    TEST_ASSERT_EQUAL(LOWERING, actual_state);

    motor_off_Expect();
    light_off_Expect();

    actual_state = gdc_process_event(FULLY_CLOSED);
    TEST_ASSERT_EQUAL(CLOSED, actual_state);
}

void test_garage_door_controller_Closed_to_Lowering_via_Button_press(void)
{
    light_on_Expect();
    motor_on_Expect(CLOCKWISE);

    actual_state = gdc_process_event(BUTTON_PRESSED);
    TEST_ASSERT_EQUAL(RAISING, actual_state);
    
    motor_off_Expect();
    light_off_Expect();

    actual_state = gdc_process_event(BUTTON_PRESSED);
    TEST_ASSERT_EQUAL(STOPPED_RAISING, actual_state);

    light_on_Expect();
    motor_on_Expect(ANTI_CLOCKWISE);

    actual_state = gdc_process_event(BUTTON_PRESSED);
    TEST_ASSERT_EQUAL(LOWERING, actual_state);
}

void test_garage_door_controller_Closed_to_Open_to_Raising_via_Button_press(void)
{
    test_garage_door_controller_Closed_to_Open();

    light_on_Expect();
    motor_on_Expect(ANTI_CLOCKWISE);

    actual_state = gdc_process_event(BUTTON_PRESSED);
    TEST_ASSERT_EQUAL(LOWERING, actual_state);

    motor_off_Expect();
    light_off_Expect();

    actual_state = gdc_process_event(BUTTON_PRESSED);
    TEST_ASSERT_EQUAL(STOPPED_LOWERING, actual_state);

    light_on_Expect();
    motor_on_Expect(CLOCKWISE);

    actual_state = gdc_process_event(BUTTON_PRESSED);
    TEST_ASSERT_EQUAL(RAISING, actual_state);
}

void test_garage_door_controller_Closed_to_Stopped_via_motor_overcurrent(void)
{
    light_on_Expect();
    motor_on_Expect(CLOCKWISE);

    actual_state = gdc_process_event(BUTTON_PRESSED);
    TEST_ASSERT_EQUAL(RAISING, actual_state);

    motor_off_Expect();
    light_off_Expect();

    actual_state = gdc_process_event(MOTOR_OVERCURRENT);
    TEST_ASSERT_EQUAL(STOPPED_RAISING, actual_state);
}

void test_garage_door_controller_Closed_to_Open_to_Stopped_via_motor_overcurrent(void)
{
    test_garage_door_controller_Closed_to_Open();

    light_on_Expect();
    motor_on_Expect(ANTI_CLOCKWISE);

    actual_state = gdc_process_event(BUTTON_PRESSED);
    TEST_ASSERT_EQUAL(LOWERING, actual_state);

    motor_off_Expect();
    light_off_Expect();

    actual_state = gdc_process_event(MOTOR_OVERCURRENT);
    TEST_ASSERT_EQUAL(STOPPED_LOWERING, actual_state);
}

void test_garage_door_controller_Closed_to_Open_to_Stopped_via_IR_beam(void)
{
    test_garage_door_controller_Closed_to_Open();

    light_on_Expect();
    motor_on_Expect(ANTI_CLOCKWISE);

    actual_state = gdc_process_event(BUTTON_PRESSED);
    TEST_ASSERT_EQUAL(LOWERING, actual_state);

    motor_off_Expect();
    light_off_Expect();

    actual_state = gdc_process_event(IR_BREAK);
    TEST_ASSERT_EQUAL(STOPPED_LOWERING, actual_state);
}

void test_Closed_Ignored_events(void)
{
    actual_state = gdc_process_event(FULLY_OPEN);
    TEST_ASSERT_EQUAL(CLOSED, actual_state);
    actual_state = gdc_process_event(FULLY_CLOSED);
    TEST_ASSERT_EQUAL(CLOSED, actual_state);
    actual_state = gdc_process_event(MOTOR_OVERCURRENT);
    TEST_ASSERT_EQUAL(CLOSED, actual_state);
    actual_state = gdc_process_event(IR_BREAK);
    TEST_ASSERT_EQUAL(CLOSED, actual_state);
}

void test_Raising_Ignored_events(void)
{
    light_on_Expect();
    motor_on_Expect(CLOCKWISE);

    gdc_process_event(BUTTON_PRESSED);    

    actual_state = gdc_process_event(FULLY_CLOSED);
    TEST_ASSERT_EQUAL(RAISING, actual_state);
    actual_state = gdc_process_event(IR_BREAK);
    TEST_ASSERT_EQUAL(RAISING, actual_state);
}

void test_Open_Ignored_events(void)
{
    test_garage_door_controller_Closed_to_Open();

    actual_state = gdc_process_event(FULLY_OPEN);
    TEST_ASSERT_EQUAL(OPEN, actual_state);
    actual_state = gdc_process_event(FULLY_CLOSED);
    TEST_ASSERT_EQUAL(OPEN, actual_state);
    actual_state = gdc_process_event(MOTOR_OVERCURRENT);
    TEST_ASSERT_EQUAL(OPEN, actual_state);
    actual_state = gdc_process_event(IR_BREAK);
    TEST_ASSERT_EQUAL(OPEN, actual_state);
}

void test_Lowering_Ignored_events(void)
{
    test_garage_door_controller_Closed_to_Open();

    light_on_Expect();
    motor_on_Expect(ANTI_CLOCKWISE);

    gdc_process_event(BUTTON_PRESSED);    

    actual_state = gdc_process_event(FULLY_OPEN);
    TEST_ASSERT_EQUAL(LOWERING, actual_state);

}

void test_Stopped_Raising_Ignored_events(void)
{
    light_on_Expect();
    motor_on_Expect(CLOCKWISE);

    gdc_process_event(BUTTON_PRESSED);  

    motor_off_Expect();
    light_off_Expect();

    gdc_process_event(BUTTON_PRESSED);  

    actual_state = gdc_process_event(FULLY_OPEN);
    TEST_ASSERT_EQUAL(STOPPED_RAISING, actual_state);
    actual_state = gdc_process_event(FULLY_CLOSED);
    TEST_ASSERT_EQUAL(STOPPED_RAISING, actual_state);
    actual_state = gdc_process_event(MOTOR_OVERCURRENT);
    TEST_ASSERT_EQUAL(STOPPED_RAISING, actual_state);
    actual_state = gdc_process_event(IR_BREAK);
    TEST_ASSERT_EQUAL(STOPPED_RAISING, actual_state);
}

void test_Stopped_Lowering_Ignored_events(void)
{
    test_garage_door_controller_Closed_to_Open();

    light_on_Expect();
    motor_on_Expect(ANTI_CLOCKWISE);

    gdc_process_event(BUTTON_PRESSED);  

    motor_off_Expect();
    light_off_Expect();
      
    gdc_process_event(BUTTON_PRESSED);  

    actual_state = gdc_process_event(FULLY_OPEN);
    TEST_ASSERT_EQUAL(STOPPED_LOWERING, actual_state);
    actual_state = gdc_process_event(FULLY_CLOSED);
    TEST_ASSERT_EQUAL(STOPPED_LOWERING, actual_state);
    actual_state = gdc_process_event(MOTOR_OVERCURRENT);
    TEST_ASSERT_EQUAL(STOPPED_LOWERING, actual_state);
    actual_state = gdc_process_event(IR_BREAK);
    TEST_ASSERT_EQUAL(STOPPED_LOWERING, actual_state);
}
