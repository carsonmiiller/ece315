/*
 * kitchen_timer.c
 *
 *  Created on: April 28, 2022
 *      Author: Carson Miller, Benjamin Waverly
 */

#include "kitchen_timer.h"
volatile int hours;
volatile int minutes;
volatile int seconds;
volatile int hours_init;
volatile int minutes_init;
volatile bool minutes_sel;
volatile bool blink;
volatile bool buzzer;
volatile bool eyes;

/******************************************************************************
 * Make sure you examine the functions that are made available to you in
 * the provided_code directory.  There are already functions provided that
 * configure timers for 1 second and 2mS.  The ISRs for these timers are
 * also provided.
 *
 * You should also make use of the functions you completed in display.c and
 * cap_sense.c
 *
 * You can add as many helper functions are you like to this file.
 *
 * DO NOT try to code all of this at once.  Break this into smaller steps.
 *
 *      -> Write  a small amount of code to accomplish a simple requirement.
 *      -> Verify your new code works.
 *      -> Verify that you did not break any of your earlier code.
 *      -> Continue to the next requirement.
 *
 * Use the debugger to verify your code!
 *
 * Suggested Development Strategy
 *
 * 1. Detect when the user presses one of the Cap Sense Buttons
 * 2. Determine which button has been pressed
 * 3. Print out a number to a single seven segment digit
 * 4. Print out a 4-digit number to the display (see video on POV)
 * 5. Blink the 4-digit number on/off at a rate of 1 second
 * 6. Modify the 4-digit number displayed using the Cap Sense Buttons
 * 6. Count down to 00:00
 * 7. Toggle the eyes/buzzer
 * 8. Complete remaining requirements.
 *****************************************************************************/

/*****************************************************
 * Allows the user to set the duration of the timer
 *****************************************************/
void kitchen_timer_mode_init(void) {
    while(1){
        // things that are done/updated every 2mS
        if(ALERT_2_MILLISECOND){
            // toggle all 4 7-segment LEDs on/off every 1s
            if(blink == false){
                display_digit(3, hours/10);
                display_digit(2, hours%10);
                display_digit(1, minutes/10);
                display_digit(0, minutes%10);
                display_all_dig_off();
            }
            uint8_t buttonstate; // holds value of button presses
            // code to execute if any buttons are pressed
            if(ALERT_BUTTON_PRESSED){
                ALERT_BUTTON_PRESSED = false;
                buttonstate = cap_sense_get_buttons(); // store current value of button presses in buttonstate
                if(buttonstate == 0x03){ // if both H + M buttons pressed
                    seconds = 0; // seconds set to 0 at start of countdown
                    // store hours and minutes at start of countdown
                    hours_init = hours;
                    minutes_init = minutes;
                    // returning will take us to countdown mode
                    return;
                }
                if(buttonstate & BIT0)  // minutes button pressed sets minutes_sel flag
                    minutes_sel = true;
                if(buttonstate & BIT1)  // hours button pressed disables minutes_sel flag
                    minutes_sel = false;
                if(buttonstate & BIT2){ // down button pressed
                    if(minutes_sel){
                        // handles rollover effect or decrements value
                        if(minutes == 0)
                            minutes = 59;
                        else
                            minutes--;
                    } else {
                        // handles rollover effect or decrements value
                        if(hours == 0)
                            hours = 99;
                        else
                            hours--;
                    }
                }
                if(buttonstate & BIT3){ // up button pressed
                    if(minutes_sel){
                        // handles rollover effect or increments value
                        if(minutes == 59)
                            minutes = 0;
                        else
                            minutes++;
                    } else {
                        // handles rollover effect or increments value
                        if(hours == 99)
                            hours = 0;
                        else
                            hours++;
                    }
                }
            }
            ALERT_2_MILLISECOND = false;
        }
        // things that are done/updated every 1s
        if(ALERT_1_SECOND){
            // toggles between blank LED screen and initialization values every second
            if(blink){
                display_all_dig_off();
                blink = false;
            }
            else{
                blink = true;
            }
            ALERT_1_SECOND = false;
        }
    }
}

/*****************************************************
 * Starts the timer
 *****************************************************/
void kitchen_timer_mode_count_down(void)
{
    while(1){
        // things that are done/updated every 2mS
        if(ALERT_2_MILLISECOND){
            ALERT_2_MILLISECOND = false;
            uint8_t buttonstate; // holds value of button presses
            // if less than an hour remaining, display minutes and seconds remaining (MM:SS)
            if(hours == 0){
                display_digit(3, minutes/10);
                display_digit(2, minutes%10);
                display_digit(1, seconds/10);
                display_digit(0, seconds%10);
            // if more than hour remaining, display hours and minutes (HH:MM)
            } else {
                display_digit(3, hours/10);
                display_digit(2, hours%10);
                display_digit(1, minutes/10);
                display_digit(0, minutes%10);
            }
            // only type of button press that matters is H + M button press
            if(ALERT_BUTTON_PRESSED){
                ALERT_BUTTON_PRESSED = false;
                buttonstate = cap_sense_get_buttons();
                // if H + M buttons pressed
                if(buttonstate == 0x03){
                    // turn off Bucky's eyes and buzzer
                    display_eye_left(false);
                    display_eye_right(false);
                    buzzer_off();
                    // set hours and minutes back to values at initialization
                    hours = hours_init;
                    minutes = minutes_init;
                    // returning takes us back ot initialization mode
                    return;
                }
            }
            // things that are done/updated every 1s
            if(ALERT_1_SECOND){
                ALERT_1_SECOND = false;
                // decrement seconds til they're 0
                if(seconds > 0)
                    seconds--;
                // if more than minute remaining, decrement minutes and rollover seconds
                if(minutes > 0 && seconds == 0){
                    minutes--;
                    seconds = 59;
                }
                // if more than hour remaining, decrement hours and rollover minutes and seconds
                if(minutes == 0 && hours > 0 && seconds == 0){
                    hours--;
                    minutes = 59;
                    seconds = 59;
                }
                // if timer expired, follow 00:00 protocol
                if(hours == 0 && minutes == 0 && seconds == 0){
                    // toggles buzzer on/off every 1s
                    if(!buzzer){
                        buzzer_on();
                        buzzer = true;
                    } else {
                        buzzer_off();
                        buzzer = false;
                    }
                    // toggles Bucky's eyes on/off every 1s
                    if(!eyes){
                        display_eye_left(true);
                        display_eye_right(true);
                        eyes = true;
                    } else {
                        display_eye_left(false);
                        display_eye_right(false);
                        eyes = false;
                    }
                }
            }
        }
    }
}
