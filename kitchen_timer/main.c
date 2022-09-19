/*
 * main.c
 *
 *  Created on: April 28, 2022
 *      Author: Carson Miller, Benjamin Waverly
 */

#include "msp.h"
#include "kitchen_timer.h"
#include "cap_sense.h"
#include "display.h"
#include "provided_code/i2c.h"
#include "provided_code/buzzer.h"
#include "provided_code/hw_timers.h"

extern volatile int minutes;
extern volatile int hours;
extern volatile int seconds;
extern volatile int hours_init;
extern volatile int minutes_init;
extern volatile bool minutes_sel;
extern volatile bool blink;
extern volatile bool buzzer;
extern volatile bool eyes;

void main(void)
{
    uint8_t buttons;
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// initialization of all peripherals
	display_init();
	buzzer_init(8000);
	buzzer_off();
	i2c_init();
	cap_sense_interrupt_init();
	buttons = cap_sense_get_buttons();
	hw_timer_init_1S_irq();
	hw_timer_init_2ms_irq();
	display_eye_left(false);
	display_eye_right(false);
    __enable_irq();

    // initialize all global clock values to 0
    hours = 0;
    minutes = 0;
    seconds = 0;
    // initialize all global toggle values to false
    minutes_sel = false;
    blink = false;
    buzzer = false;
    eyes = false;

    // timer operation
    while(1){
        kitchen_timer_mode_init();
        kitchen_timer_mode_count_down();
    };
}
