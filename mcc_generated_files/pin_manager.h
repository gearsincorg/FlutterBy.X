/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC16LF15324
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.05 and above
        MPLAB 	          :  MPLAB X 5.20	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set MOTION aliases
#define MOTION_TRIS                 TRISAbits.TRISA2
#define MOTION_LAT                  LATAbits.LATA2
#define MOTION_PORT                 PORTAbits.RA2
#define MOTION_WPU                  WPUAbits.WPUA2
#define MOTION_OD                   ODCONAbits.ODCA2
#define MOTION_ANS                  ANSELAbits.ANSA2
#define MOTION_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define MOTION_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define MOTION_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define MOTION_GetValue()           PORTAbits.RA2
#define MOTION_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define MOTION_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define MOTION_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define MOTION_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define MOTION_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define MOTION_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define MOTION_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define MOTION_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set MIC aliases
#define MIC_TRIS                 TRISAbits.TRISA4
#define MIC_LAT                  LATAbits.LATA4
#define MIC_PORT                 PORTAbits.RA4
#define MIC_WPU                  WPUAbits.WPUA4
#define MIC_OD                   ODCONAbits.ODCA4
#define MIC_ANS                  ANSELAbits.ANSA4
#define MIC_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define MIC_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define MIC_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define MIC_GetValue()           PORTAbits.RA4
#define MIC_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define MIC_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define MIC_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define MIC_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define MIC_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define MIC_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define MIC_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define MIC_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set SW_VCC aliases
#define SW_VCC_TRIS                 TRISAbits.TRISA5
#define SW_VCC_LAT                  LATAbits.LATA5
#define SW_VCC_PORT                 PORTAbits.RA5
#define SW_VCC_WPU                  WPUAbits.WPUA5
#define SW_VCC_OD                   ODCONAbits.ODCA5
#define SW_VCC_ANS                  ANSELAbits.ANSA5
#define SW_VCC_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define SW_VCC_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define SW_VCC_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define SW_VCC_GetValue()           PORTAbits.RA5
#define SW_VCC_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define SW_VCC_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define SW_VCC_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define SW_VCC_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define SW_VCC_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define SW_VCC_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define SW_VCC_SetAnalogMode()      do { ANSELAbits.ANSA5 = 1; } while(0)
#define SW_VCC_SetDigitalMode()     do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set VIB_L aliases
#define VIB_L_TRIS                 TRISCbits.TRISC0
#define VIB_L_LAT                  LATCbits.LATC0
#define VIB_L_PORT                 PORTCbits.RC0
#define VIB_L_WPU                  WPUCbits.WPUC0
#define VIB_L_OD                   ODCONCbits.ODCC0
#define VIB_L_ANS                  ANSELCbits.ANSC0
#define VIB_L_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define VIB_L_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define VIB_L_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define VIB_L_GetValue()           PORTCbits.RC0
#define VIB_L_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define VIB_L_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define VIB_L_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define VIB_L_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define VIB_L_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define VIB_L_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define VIB_L_SetAnalogMode()      do { ANSELCbits.ANSC0 = 1; } while(0)
#define VIB_L_SetDigitalMode()     do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set VIB_R aliases
#define VIB_R_TRIS                 TRISCbits.TRISC1
#define VIB_R_LAT                  LATCbits.LATC1
#define VIB_R_PORT                 PORTCbits.RC1
#define VIB_R_WPU                  WPUCbits.WPUC1
#define VIB_R_OD                   ODCONCbits.ODCC1
#define VIB_R_ANS                  ANSELCbits.ANSC1
#define VIB_R_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define VIB_R_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define VIB_R_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define VIB_R_GetValue()           PORTCbits.RC1
#define VIB_R_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define VIB_R_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define VIB_R_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define VIB_R_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define VIB_R_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define VIB_R_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define VIB_R_SetAnalogMode()      do { ANSELCbits.ANSC1 = 1; } while(0)
#define VIB_R_SetDigitalMode()     do { ANSELCbits.ANSC1 = 0; } while(0)

// get/set LED_R aliases
#define LED_R_TRIS                 TRISCbits.TRISC2
#define LED_R_LAT                  LATCbits.LATC2
#define LED_R_PORT                 PORTCbits.RC2
#define LED_R_WPU                  WPUCbits.WPUC2
#define LED_R_OD                   ODCONCbits.ODCC2
#define LED_R_ANS                  ANSELCbits.ANSC2
#define LED_R_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define LED_R_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define LED_R_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define LED_R_GetValue()           PORTCbits.RC2
#define LED_R_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define LED_R_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define LED_R_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define LED_R_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define LED_R_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define LED_R_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define LED_R_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define LED_R_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set LED_L aliases
#define LED_L_TRIS                 TRISCbits.TRISC3
#define LED_L_LAT                  LATCbits.LATC3
#define LED_L_PORT                 PORTCbits.RC3
#define LED_L_WPU                  WPUCbits.WPUC3
#define LED_L_OD                   ODCONCbits.ODCC3
#define LED_L_ANS                  ANSELCbits.ANSC3
#define LED_L_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define LED_L_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define LED_L_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define LED_L_GetValue()           PORTCbits.RC3
#define LED_L_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define LED_L_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define LED_L_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define LED_L_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define LED_L_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define LED_L_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define LED_L_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define LED_L_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set FINGER_L aliases
#define FINGER_L_TRIS                 TRISCbits.TRISC4
#define FINGER_L_LAT                  LATCbits.LATC4
#define FINGER_L_PORT                 PORTCbits.RC4
#define FINGER_L_WPU                  WPUCbits.WPUC4
#define FINGER_L_OD                   ODCONCbits.ODCC4
#define FINGER_L_ANS                  ANSELCbits.ANSC4
#define FINGER_L_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define FINGER_L_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define FINGER_L_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define FINGER_L_GetValue()           PORTCbits.RC4
#define FINGER_L_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define FINGER_L_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define FINGER_L_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define FINGER_L_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define FINGER_L_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define FINGER_L_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define FINGER_L_SetAnalogMode()      do { ANSELCbits.ANSC4 = 1; } while(0)
#define FINGER_L_SetDigitalMode()     do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set FINGER_R aliases
#define FINGER_R_TRIS                 TRISCbits.TRISC5
#define FINGER_R_LAT                  LATCbits.LATC5
#define FINGER_R_PORT                 PORTCbits.RC5
#define FINGER_R_WPU                  WPUCbits.WPUC5
#define FINGER_R_OD                   ODCONCbits.ODCC5
#define FINGER_R_ANS                  ANSELCbits.ANSC5
#define FINGER_R_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define FINGER_R_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define FINGER_R_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define FINGER_R_GetValue()           PORTCbits.RC5
#define FINGER_R_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define FINGER_R_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define FINGER_R_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define FINGER_R_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define FINGER_R_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define FINGER_R_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define FINGER_R_SetAnalogMode()      do { ANSELCbits.ANSC5 = 1; } while(0)
#define FINGER_R_SetDigitalMode()     do { ANSELCbits.ANSC5 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCAF2 pin functionality
 * @Example
    IOCAF2_ISR();
 */
void IOCAF2_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCAF2 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCAF2 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCAF2_SetInterruptHandler(MyInterruptHandler);

*/
void IOCAF2_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCAF2 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCAF2_SetInterruptHandler() method.
    This handler is called every time the IOCAF2 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCAF2_SetInterruptHandler(IOCAF2_InterruptHandler);

*/
extern void (*IOCAF2_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCAF2 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCAF2_SetInterruptHandler() method.
    This handler is called every time the IOCAF2 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCAF2_SetInterruptHandler(IOCAF2_DefaultInterruptHandler);

*/
void IOCAF2_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/