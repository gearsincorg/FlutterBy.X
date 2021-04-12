/**
  ADC Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    adc.c

  @Summary
    This is the generated driver implementation file for the ADC driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides implementations for driver APIs for ADC.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC16LF15324
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.05 and above
        MPLAB             :  MPLAB X 5.20
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "adc.h"
#include "device_config.h"

/**
  Section: Macro Declarations
*/

#define ACQ_US_DELAY 5

/**
  Section: ADC Module APIs
*/

void ADC_Initialize(void)
{
    // set the ADC to the options selected in the User Interface
    
    // GOnDONE stop; ADON enabled; CHS ANA4; 
    ADCON0 = 0x11;
    
    // ADFM right; ADPREF VDD; ADCS FOSC/16; 
    ADCON1 = 0xD0;
    
    // ADRESL 0; 
    ADRESL = 0x00;
    
    // ADRESH 0; 
    ADRESH = 0x00;
    
    // Enabling ADC interrupt.
    PIE1bits.ADIE = 1;
}

void ADC_SelectChannel(adc_channel_t channel)
{
    // select the A/D channel
    ADCON0bits.CHS = channel;    
    // Turn on the ADC module
    ADCON0bits.ADON = 1;  
}

void ADC_StartConversion()
{
    // Start the conversion
    ADCON0bits.GOnDONE = 1;
}


bool ADC_IsConversionDone()
{
    // Start the conversion
   return ((bool)(!ADCON0bits.GOnDONE));
}

adc_result_t ADC_GetConversionResult(void)
{
    // Conversion finished, return the result
    return ((adc_result_t)((ADRESH << 8) + ADRESL));
}

adc_result_t ADC_GetConversion(adc_channel_t channel)
{
    // select the A/D channel
    ADCON0bits.CHS = channel;    
    
    // Turn on the ADC module
    ADCON0bits.ADON = 1;

    // Acquisition time delay
    __delay_us(ACQ_US_DELAY);

    // Start the conversion
    ADCON0bits.GOnDONE = 1;

    // Wait for the conversion to finish
    while (ADCON0bits.GOnDONE)
    {
    }

    // Conversion finished, return the result
    return ((adc_result_t)((ADRESH << 8) + ADRESL));
}

void ADC_TemperatureAcquisitionDelay(void)
{
    __delay_us(200);
}

// USER Code goes here ==========================
// keep track of 25 mS sound samples
// For each sample, detect if peak level is greater than TRIGGER threshold.
// Count number of trigger samples per second.
// keep a sliding window of second totals
// Keep count of number of trigger samples per window.

#include "string.h"

#define ACC_TRIGGER_COUNT   200
#define ACC_WARNING_COUNT   100

#define SAMPLE_COUNT  400        // 25 mSec total  (400 x 62.5 uS))
//#define WARNING_LEVEL 0x009C     // 0x2710UL   // 75 dB @ 2 kHz
//#define TRIGGER_LEVEL 0x012C     // 0x4B00UL   // 80 dB @ 2 kHz
#define SAMPLES_PER_INT  10      // 250 mSec samples
#define SLIDING_WINDOW_INT 40    // 10 Sec total window

#define NUM_MODES         5U      // also in main.h

uint16_t triggerLevels[NUM_MODES + 2] = {35, 70, 156, 300, 720, 1400, 2800};

uint16_t warningLevel = 0;
uint16_t triggerLevel = 0;

uint8_t triggersEachInt[SLIDING_WINDOW_INT];
uint8_t sampleNumber    = 0;
uint16_t sampleTriggers  = 0;
uint8_t secondIndex     = 0;
uint16_t totalTriggers  = 0;

uint16_t samples = 0;
uint16_t volume  = 0;

uint16_t minLevel   = 0xFFFFUL;
uint16_t maxLevel   = 0x0000UL;

bool    runTriggers = true;
uint16_t ramp = 0x00;

void ADC_ISR(void)
{
    calculateVolume(ADC_GetConversionResult()) ;
            
    // Clear the ADC interrupt flag
    PIR1bits.ADIF = 0;
    ADC_StartConversion();
}

void    resetVolumeLimit()
{
    volume = 0;
    minLevel   = 0xFFFFUL;
    maxLevel   = 0x0000UL;
    sampleNumber    = 0;
    sampleTriggers  = 0;
    secondIndex     = 0;
    totalTriggers  = 0;
    memset(triggersEachInt, 0, sizeof(triggersEachInt));
}

void    startTriggers() {
    runTriggers = true;
}

void    stopTriggers() {
    runTriggers = false;
}

void calculateVolume(uint16_t level) 
{
    if (runTriggers) {

        if (level > maxLevel)
            maxLevel = level;
        if (level < minLevel)
            minLevel = level;

        // is it time to process this sample (each 50 mSec)
        if (samples++ >= SAMPLE_COUNT) {
            volume = (uint16_t)(maxLevel - minLevel);

            // bump the triggers for this second if the sample is above the trigger leveL>
            if (volume > triggerLevel)
                sampleTriggers += 2;
            else if (volume > warningLevel)
                sampleTriggers += 1;

            // Bump the sample and check if the second is finished)
            if (++sampleNumber >= SAMPLES_PER_INT) {
                // subtract the oldest second and add the new one.
                totalTriggers -= triggersEachInt[secondIndex];
                triggersEachInt[secondIndex] = sampleTriggers;
                totalTriggers += sampleTriggers;     
                
                if (totalTriggers > 25 )
                    setRightLED(totalTriggers << 1);
                else
                    setRightLED(0);

                if (++secondIndex == SLIDING_WINDOW_INT){
                    secondIndex = 0;
                }

                sampleTriggers = 0;
                sampleNumber  = 0;
            }

            samples   = 0;
            minLevel  = 0xFFFFUL;
            maxLevel  = 0x0000UL;
        }
    }
}

int    checkVolume()
{
    if (totalTriggers > ACC_TRIGGER_COUNT)
        return 2;
    else if (totalTriggers > ACC_WARNING_COUNT)
        return 1;
    else
        return (0);
}

void    setModeLevels(uint8_t mode) {
    warningLevel = triggerLevels[mode];
    triggerLevel = triggerLevels[mode + 2U];
}

/**
 End of File
*/