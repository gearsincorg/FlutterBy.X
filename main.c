/**
 * Generated Main Source File
 *
 * Company:
 *   Microchip Technology Inc.
 *
 * File Name:
 *   main.c
 * 
 * Version  Date        Comment 
 * 1.0      6/1/2021    Original release to First Client Jocelynn
 * 2.0      7/10/2021   Updated for Rev 2.0 Hardware: Super Cap testing
 * 2.3      8/14/2021   Added new tickle, LED indication and Tickle hold-off
 * 
 * Summary:
*/

#include "mcc_generated_files/mcc.h"
#include "main.h"

#define PUBLIC_REV  2
#define PUBLIC_MINOR  2

// User Code
uint24_t timerMS = 0;
uint24_t sleepMS = 0;
uint24_t edgeTime = 0;
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

uint24_t tickleHoldoff    = 0;   

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
    
    LED_GREEN_SetHigh();
    LED_ORANGE_SetHigh();
    VIBRATE_SetHigh();
    CHARGE_SetHigh();
    SW_VCC_SetHigh();

    setOrangeLED(0);
    setGreenLED(0);
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
            if (!buttonPressed(false) && shortPress) {+
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

                // clear out, and ignore short presses
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
                if (triggerNow()) {
                    if (getTimerMS() > tickleHoldoff){
                        tickle();
                    } else {
                        // We are in holdoff
                        setOrangeLED(FULL_PWM);
                        delay(200);
                        setOrangeLED(0);
                    }
                }
            }

            // Check for a blink every 10 seconds
            if (blinkCounter++ > 200) {
                saved = currentRightLED;
                setGreenLED(0x10);
                delay(50);
                setGreenLED(saved);
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
    // Pulse the Motor
    setGreenLED(0);

    // custom Vibrate
    setOrangeLED(FULL_PWM);
    setVibrate(FULL_PWM);
    delay(250);
    setOrangeLED(0);
    setVibrate(0);
    delay(300);

    setOrangeLED(FULL_PWM);
    setVibrate(FULL_PWM);
    delay(150);
    setOrangeLED(0);
    setVibrate(0);
    delay(200);

    setOrangeLED(FULL_PWM);
    setVibrate(FULL_PWM);
    delay(250);
    setOrangeLED(0);
    setVibrate(0);
   
    delay(1000);
    setModeLevels(currentMode);
    resetVolumeLimit();
    
    tickleHoldoff = getTimerMS() + TICKLE_HOLDOFF_MS ;
            
    startTriggers();
}


void    delay(uint24_t delayMS)
{
    uint24_t stop = getTimerMS() + delayMS;
    while (getTimerMS() < stop) ;
}

void    setVibrate(uint16_t intensity){
    PWM4_LoadDutyValue(intensity);
}

void    setOrangeLED(uint16_t brightness){
    currentLeftLED = brightness;
    PWM6_LoadDutyValue(brightness);
}

void    setGreenLED(uint16_t brightness){
    currentRightLED = brightness;
    PWM5_LoadDutyValue(brightness);
}

void motionHandler(void){
    if (MOTION_GetValue() == 0) {
        resetSleepMS();
    }
}

bool leftPressed() {

    bool leftButton = (FINGER_L_GetValue() == 0);
    
    if (getTimerMS() > (edgeTime + DEBOUNCE)) {
        if (leftButton != lastLeftButton) {
            lastLeftButton = leftButton;
            edgeTime = getTimerMS() ;
        }
    }
    return (lastLeftButton);
}

bool rightPressed() {
    return (FINGER_R_GetValue() == 0);
}

// Indicate the duration the button is down for
bool    buttonPressed( bool poweringDown) {
    
    // Are we holding the button down right now.
    if (leftPressed()) {
        stopTriggers();
        if (getTimerMS() < (edgeTime + LONG_PRESS)) {
            shortPress = true;
        } else {
            if (poweringDown && !longPress) {
                      click();
            }
            shortPress = false;
            longPress  = true;
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
    delay(75);
    setVibrate(0);
    delay(150);
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
            setOrangeLED(b);
            delay(10);
        }
    }
    setOrangeLED(0);
    setGreenLED(0);
    setVibrate(0);
    
    NOP();
    SLEEP();  // Power down the PIC
    NOP();

    //Waking up
    delay(10);
    clearPresses();
    edgeTime = getTimerMS();

    // Waking up.
    SW_VCC_SetHigh();
    ADC_StartConversion();
    resetVolumeLimit();
    startTriggers();

    brightness = 0;
    while (brightness < QUAT_PWM) {
        brightness++;
        setOrangeLED(brightness);
        delay(10);
    }
    setOrangeLED(0);

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
    setOrangeLED(0);
    setGreenLED(0);
    setVibrate(0);

    resetTimerMS();
    while ((getTimerMS() < MUTE_TIMER)) {
        if (buttonPressed(false) && longPress) {
            click();
            click();
            setOrangeLED(QUAT_PWM);
            break;
        }
        delay(20);
    }
    
    // Waking up.
    SW_VCC_SetHigh();
    // Show LED if auto wake
    if (getTimerMS() >= MUTE_TIMER) {
        for (brightness = 0; brightness < QUAT_PWM; brightness++) {
            setOrangeLED(brightness);
            delay(2);
        }
    }
    setOrangeLED(0);
            
    // wait for button release
    while(buttonPressed(false)) {
        delay(5);
    }
    clearPresses();

    setOrangeLED(0);
    startTriggers();
    resetVolumeLimit();
}


void    flashRev() {
    
    // Blink the Rev number
    for (int r=0; r < PUBLIC_REV; r++) {
        setOrangeLED(HALF_PWM);  // Green
        
        delay(300);
        setOrangeLED(0);
        delay(400);
    }

    for (int r=0; r < PUBLIC_MINOR; r++) {
        setGreenLED(HALF_PWM);   // Blue
        delay(100);
        setGreenLED(0);
        delay(100);
    }
}

void    flashMode() {
    
    // Blink the Mode number
    switch (modeState) {
        case 0:  // number is off
            if (timerMS > modeTime) {
                setGreenLED(HALF_PWM);
                modeTime = timerMS + 200;
                modeState = 1;
            }
            break;
            
        case 1:  // number is on
            if (timerMS > modeTime) {
                setGreenLED(0);
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
    edgeTime = timerMS ;
    tickleHoldoff = 0;
}

void resetSleepMS()
{
    blinkCounter = 0;
    sleepMS = 0UL;   
}

/**
 End of File
*/