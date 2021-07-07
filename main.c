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

#define PUBLIC_REV  1
#define PUBLIC_MINOR  1

// User Code
uint24_t timerMS = 0;
uint24_t sleepMS = 0;
uint24_t pressTime = 0;
uint24_t modeTime  = 0;
bool     longPress = false;
bool     shortPress = false;
bool     changePress = false;
bool     beaconIsRed = false;
uint8_t  patternIndex = 0;
uint16_t brightness = 0;
bool     lastLeftButton = false;
bool     lastRightButton = false;

uint8_t  currentMode = 3;
uint8_t  modeState   = 0;
uint8_t  modeCount   = 0;

uint16_t blinkCounter = 0;

uint16_t currentLeftLED = 0;
uint16_t currentRightLED = 0;

/*
                         Main application
 */
void main(void)
{
    uint8_t volume;
    uint16_t saved;
    
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
    setLeftLED(0);
    setRightLED(0);
    SW_VCC_SetHigh();
    setVibrate(0);

    TMR0_StartTimer();
    resetVolumeLimit();
    resetTimerMS();
    powerUpTest();
    
    ADC_StartConversion();
    setModeLevels(currentMode);
    
    while (1)
    {
        if (rightPressed()) {
            stopTriggers();
            flashMode();
            buttonPressed(true);
            
            if (longPress) {
                currentMode++ ;
                if (currentMode >= NUM_MODES)
                    currentMode = 0;
                
                setModeLevels(currentMode);
                resetVolumeLimit();
                
                delay (100);
                resetTimerMS();
                clearPresses();
            }
                
        } else  {
            startTriggers();
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
            }

            // Check for a blink every 10 seconds
            if (blinkCounter++ > 200) {
                saved = currentRightLED;
                setRightLED(0x10);
                delay(50);
                setRightLED(saved);
                blinkCounter = 0;
            } else {
                delay(50);
            }
        }
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
    setRightLED(0);
    stopTriggers();
    setLeftLED(loud);
    setVibrate(loud);
    delay(200);
    setLeftLED(0);
    setVibrate(0);
    delay(100);
    
    for (int blink=0; blink < 2; blink++) {
        setLeftLED(loud);
        setVibrate(loud);
        delay(100);
        setLeftLED(0);
        setVibrate(0);
        delay(100);
    }
    
    setLeftLED(0);
    setVibrate(0);
    delay(500);
    setModeLevels(currentMode);
    resetVolumeLimit();
    startTriggers();
}


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
    currentLeftLED = brightness;
    PWM6_LoadDutyValue(brightness);
}

void    setRightLED(uint16_t brightness){
    currentRightLED = brightness;
    PWM5_LoadDutyValue(brightness);
}

void motionHandler(void){
    if (MOTION_GetValue() == 0) {
        resetSleepMS();
    }
}

bool leftPressed() {
    bool press = (FINGER_L_GetValue() == 0);
    setLeftLED(press ? 0x1FF : 0);

    // ignore a bounce or double press
    if (press && !lastLeftButton) {

        if (!(shortPress || longPress || changePress)) 
        {
            pressTime = getTimerMS();
            resetSleepMS();  // probably.....
        }
    }
    lastLeftButton = press;

    return (press);
}

bool rightPressed() {
    return (FINGER_R_GetValue() == 0);
}

// Indicate the duration the button is down for
bool    buttonPressed( bool poweringDown) {
    
    // Are we holding the button down right now.
    if (leftPressed()) {
        stopTriggers();
        if (getTimerMS() > (pressTime + LONG_PRESS)) {
            if (poweringDown && !longPress) {
                      click();
            }
  
            longPress  = true;
            shortPress = false;
        }
        else {
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
    blinkCounter = 0;
}

void    powerUpTest () {
    delay(200);
    flashRev();
    delay(500);
}

void    click() {
    setVibrate(0x3FF);
    delay(150);
    setVibrate(0);
    delay(75);
}

/*
 * Do everything required to enter super low power (OFF) mode.
 * We are woken up by another long press
 */
void    powerDown(bool timeout) {
    
    SW_VCC_SetLow();
    stopTriggers();

    // Fade out the LED
    if (timeout) {
        for (uint16_t b = QUAT_PWM; b != 0; b-- )
        {
            setLeftLED(b);
            delay(10);
        }
    }
    setLeftLED(0);
    setRightLED(0);
    setVibrate(0);
    
    NOP();
    SLEEP();  // Power down the PIC
    NOP();

    //Waking up
    delay(10);
    clearPresses();
    pressTime = getTimerMS();

    // Waking up.
    SW_VCC_SetHigh();
    ADC_StartConversion();
    resetVolumeLimit();
    startTriggers();

    brightness = 0;
    while (brightness < QUAT_PWM) {
        brightness++;
        setLeftLED(brightness);
        delay(10);
    }
    setLeftLED(0);

    resetVolumeLimit();
    startTriggers();
    resetTimerMS();
    resetSleepMS();
}

void    goToSleep(void) {
    // go offline for specific time.
    // Don't respond to noise or vibration.
    // But wake up with a long press
    stopTriggers();
    SW_VCC_SetLow();
    setLeftLED(0);
    setRightLED(0);
    setVibrate(0);

    resetTimerMS();
    while ((getTimerMS() < MUTE_TIMER)) {
        if (leftPressed() && ((getTimerMS() - pressTime) > LONG_PRESS)) {
            click();
            click();
            break;
        }
        delay(20);
    }
    
    // Waking up.
    SW_VCC_SetHigh();
    // Show LED if auto wake
    if (getTimerMS() >= MUTE_TIMER) {
        for (brightness = 0; brightness < QUAT_PWM; brightness++) {
            setLeftLED(brightness);
            delay(2);
        }
    }
    setLeftLED(0);
            
    // wait for button release
    while(leftPressed()) {
        delay(5);
    }

    setLeftLED(0);
    startTriggers();
    resetVolumeLimit();
}


void    flashRev() {
    
    // Blink the Rev number
    for (int r=0; r < PUBLIC_REV; r++) {
        setRightLED(HALF_PWM);  // Green
        
        delay(300);
        setRightLED(0);
        delay(400);
    }

    for (int r=0; r < PUBLIC_MINOR; r++) {
        setRightLED(HALF_PWM);   // Blue
        delay(100);
        setRightLED(0);
        delay(100);
    }
}

void    flashMode() {
    
    // Blink the Mode number
    switch (modeState) {
        case 0:  // number is off
            if (timerMS > modeTime) {
                setRightLED(HALF_PWM);
                modeTime = timerMS + 200;
                modeState = 1;
            }
            break;
            
        case 1:  // number is on
            if (timerMS > modeTime) {
                setRightLED(0);
                modeCount++;
                        
                if (modeCount <= currentMode) {
                    modeTime = timerMS + 300;
                }
                else {
                    modeTime = timerMS + 1000;
                    modeCount = 0;
                }
                modeState = 0;
            }
            break;
    }
    delay(5);
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
    blinkCounter = 0;
    timerMS  =  2000UL;  // Must be longer than Long press
    modeTime = timerMS;
}

void resetSleepMS()
{
    blinkCounter = 0;
    sleepMS = 0UL;   
}

/**
 End of File
*/