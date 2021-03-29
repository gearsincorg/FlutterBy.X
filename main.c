/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC16LF15324
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "main.h"

#define FULL_GREEN  24
#define PUBLIC_REV  1
#define PUBLIC_MINOR  1

// User Code
uint24_t timerMS = 0;
uint24_t sleepMS = 0;
uint24_t pressTime = 0;
bool     longPress = false;
bool     shortPress = false;
bool     changePress = false;
bool     beaconIsRed = false;
uint8_t  patternIndex = 0;
uint8_t  brightness = 0;

/*
                         Main application
 */
void main(void)
{
    uint8_t volume;
    
    // initialize the device
    SYSTEM_Initialize();
    
    IOCAF2_SetInterruptHandler(motionHandler);
    TMR1_SetInterruptHandler(timerMSHandler);
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
    
    LED_L_SetHigh();
    LED_R_SetHigh();
    VIB_L_SetHigh();
    VIB_R_SetHigh();
    SW_VCC_SetHigh();
    resetVolumeLimit();
    resetTimerMS();
    powerUpTest();
    
    while (1)
    {
        // Update normal beacon if button not pressed.
        buttonHandler();
        if (!buttonPressed(true)){

            // restart sleep timer
            if (shortPress) {
                resetTimerMS();
                clearPresses();
            }

            // Power Down
            if (longPress) {
                resetVolumeLimit();
                clearPresses();
                goToSleep();
                resetTimerMS();
                clearPresses();
            }

            // Battery Saver?
            if (sleepMS > BATTERY_SAVER){
                powerDown(true);
                resetSleepMS();
            }
                
            // Time to vibrate?
            switch (checkVolume()) {
                case 0:
                default:
                    break;
                    
                case 1:
                    break;
                    
                case 2:
                    tickle();
                    break;
            }
            
            // Update blink display
            // sineLED();
        }
        delay(50);
    }

    // Disable the Global Interrupts
    INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptDisable();
}

void    tickle() 
{
    uint16_t loud = FULL_PWM;
    
    // Pulse the Motor
    setVibrate(loud);
    delay(40);
    setVibrate(0);
    delay(80);
    
    for (int blink=0; blink < 3; blink++) {
        setVibrate(loud);
        delay(20);
        setVibrate(0);
        delay(100);
        loud >>= 2;
    }
    delay(1000);
    setVibrate(0);
    resetVolumeLimit();
}

/*
void    pulseLED()
{
    setFlag(beaconIsRed, pulse[patternIndex]);
    patternIndex = (patternIndex + 1) % sizeof(pulse);
}
*/

void    delay(uint24_t delayMS)
{
    uint24_t stop = getTimerMS() + delayMS;
    
    while (getTimerMS() < stop) ;
}

void    setVibrate(uint16_t intensity){
    PWM3_LoadDutyValue(intensity);
    PWM4_LoadDutyValue(intensity);
}

void    setLeftLED(uint16_t brightness){
    PWM6_LoadDutyValue(brightness);
}

void    setRightLED(uint16_t brightness){
    PWM5_LoadDutyValue(brightness);
}

void motionHandler(void){
    if (MOTION_GetValue() == 0) {
        resetSleepMS();
    }
}

void buttonHandler(void){
    // ignore a bounce or double press
    if (shortPress || longPress || changePress) 
        return;
    
    if (FINGER_L_GetValue() == 0) {
        pressTime = getTimerMS();
        resetSleepMS();  // probably.....
    }
}

// Indicate the duration the button is down for
bool    buttonPressed( bool poweringDown) {
    // Are we holding the button down right now.
    if (FINGER_L_GetValue() == 0) {
        stopTriggers();
        if ((getTimerMS() - pressTime) > LONG_PRESS) {
            if (poweringDown) {
                if (brightness > 0)
                    brightness--;
                setLeftLED(brightness);
            }
            else {
                if (brightness < FULL_GREEN)
                    brightness++;
                setLeftLED(brightness);
            }
            longPress  = true;
            shortPress = false;
        }
        else {
            //if (poweringDown) 
            brightness = FULL_GREEN ;           
            //else
            //    brightness = 0 ;           
                    
            //setLED(0,brightness,0);
            shortPress = true;
        }
        return true;
    }
    else {
        startTriggers(); 
        return false;
    }
}

void    clearPresses() {
    shortPress  = false;
    longPress   = false;
    changePress = false;
}

void    powerUpTest () {
    delay(200);
    flashRev(0);
    delay(500);
}

/*
 * Do everything required to enter super low power (OFF) mode.
 * We are woken up by another long press
 */
void    powerDown(bool timeout) {
    
    SW_VCC_SetLow();

    // Fade out the Green LED
    if (timeout) {
        for (uint16_t b = QUAT_PWM; b != 0; b-- )
        {
            setLeftLED(b);
            delay(20);
        }
    }
    setLeftLED(0);
    
    while (1) {
        NOP();
        SLEEP();  // Power down the PIC
        NOP();
       
        //Waking up
        delay(100);
        clearPresses();
        pressTime = getTimerMS();

        // We have woken up, so wait for button release.
        while (buttonPressed(false)) {
            delay(50);
        };
        setLeftLED(0);

        // was this a long press
        if (longPress) {
            longPress = false;
            break;
        }
        else {
            shortPress = false;
            brightness = 0;
            while (brightness < QUAT_PWM) {
                brightness++;
                setLeftLED(brightness);
                delay(20);
            }
            setLeftLED(0);
            break;
        }
    }

    // Waking up.
    SW_VCC_SetHigh();
    resetVolumeLimit();
    delay(250);
    resetTimerMS();
    clearPresses();
}

void    goToSleep(void) {
    // go offline for specific time.
    // Don't respond to noise or vibration.
    // But wake up with a long press
    stopTriggers();
    SW_VCC_SetLow();

    resetTimerMS();
    while ((getTimerMS() < MUTE_TIMER)) {
        if ((FINGER_L_GetValue() == 0) && ((getTimerMS() - pressTime) > LONG_PRESS)) 
            break;
        delay(20);
    }
    
    // Waking up.
    SW_VCC_SetHigh();
    for (brightness = 0; brightness < QUAT_PWM; brightness++) {
        setLeftLED(brightness);
        delay(20);
    }
    
    // wait for button release
    while(FINGER_L_GetValue() == 0) {
        delay(5);
    }

    setLeftLED(0);
    startTriggers();
    resetVolumeLimit();
}


void    flashRev(uint8_t greenLevel) {
    
    // Blink the Rev number
    for (int r=0; r < PUBLIC_REV; r++) {
        setRightLED(HALF_PWM);  // Green
        
        delay(200);
        setRightLED(0);
        delay(300);
    }

    for (int r=0; r < PUBLIC_MINOR; r++) {
        setRightLED(QUAT_PWM);   // Blue
        delay(50);
        setRightLED(0);
        delay(100);
    }
}

// TIMER CODE
void timerMSHandler(void)
{
    timerMS++;
    sleepMS++;
}

uint24_t getTimerMS()
{
    return timerMS;
}

void resetTimerMS()
{
    timerMS = 10000UL;  // Must be longer than Long press
}

void resetSleepMS()
{
    sleepMS = 0UL;   
}

/**
 End of File
*/