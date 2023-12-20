#include "IOs.h"
#include "TimeDelay.h"
#include "xc.h"
#include "UART2.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>

extern int RA2flag;
extern int RA4flag;
extern int RB4flag;
extern int delayFlag;
extern int displayFlag;

void IOinit(void) {
    //Clock output on REFO pin - pin 18 RB15
     TRISBbits.TRISB15 = 0;  // Set RB15 as output for REFO
     REFOCONbits.ROEN = 1; // Ref oscillator is enabled
     REFOCONbits.ROSSLP = 0; // Ref oscillator is disabled in sleep
     REFOCONbits.ROSEL = 0; // Output base clk showing clock switching
     REFOCONbits.RODIV = 0b0000;
     
     //IO Inititalizations
     AD1PCFG = 0xFFFF;  // Turn all analog pins to digital
     CNPU1bits.CN0PUE = 1; // Enables pull up resistor on RA4/CN0
     CNPU1bits.CN1PUE = 1; // Enables pull up resistor on RB4/CN1
     CNPU2bits.CN30PUE = 1; // Enables pull up resistor on RA2/CN30
     CNEN1bits.CN0IE = 1; // Enable CN on RA4
     CNEN1bits.CN1IE = 1; // Enable CN on RB4
     CNEN2bits.CN30IE = 1; // Enable CN on RA2

     
     // Configure CN Interrupts
     IPC4bits.CNIP = 6; // set CN priority
     IEC1bits.CNIE = 1; //enable CN interrup
     IFS1bits.CNIF = 0; //clear CN flag
    
     // set input pins to buttons
     TRISAbits.TRISA2 = 1;
     TRISAbits.TRISA4 = 1;
     TRISBbits.TRISB4 = 1;
     
     // set output pin to LED
     TRISBbits.TRISB8 = 0;
}


void IOcheck() {
    while(1)  // infinite while loop
     {
        // make delays possible again
        delayFlag = 1;
        
        // check which buttons are being pressed
        if (RA2flag + RA4flag + RB4flag == 3) {
            // all three buttons being pressed results in a solid led
            // set to idle mode as nothing is changing
            Disp2String("\n\r All PBs pressed");
                
            LATBbits.LATB8 = 1;
            Idle();
        }
        else if (RA2flag + RA4flag + RB4flag == 2) {
            // two buttons being pressed also causes a solid led with no 
            // blinking. Set to idle mode as nothing is changing
            if (!RB4flag){
                Disp2String("\n\r PB 1 and PB 2 pressed");
            }
            if (!RA2flag){
                Disp2String("\n\r PB 2 and PB 3 pressed");
            }
            if (!RA4flag){
                Disp2String("\n\r PB 1 and PB 3 pressed");
            }
 
            LATBbits.LATB8 = 1;
            Idle();
        }
        else if (RA2flag == 1) {
            // sets delay for delayFunct to 1 sec  and turns led on and off
            if (displayFlag){
                Disp2String("\n\r PB1 is pressed");
                displayFlag = 0;
            }
            LATBbits.LATB8 = 1;
            delay_ms(500);
            LATBbits.LATB8 = 0;
            delay_ms(500);
        }
        else if (RA4flag == 1) {
            // sets delay for delayFunct to 2 sec  and turns led on and off
            if (displayFlag){
                Disp2String("\n\r PB2 is pressed");
                displayFlag = 0;
            }
            LATBbits.LATB8 = 1;
            delay_ms(2000);
            LATBbits.LATB8 = 0;
            delay_ms(2000);
        }
        else if (RB4flag == 1) {
            if (displayFlag){
                Disp2String("\n\r PB3 is pressed");
                displayFlag = 0;
            }
            // sets delay for delayFunct to 3 sec  and turns led on and off
            LATBbits.LATB8 = 1;
            delay_ms(3000);
            LATBbits.LATB8 = 0;
            delay_ms(3000);
        }
        else {
            // turns the LED off if no buttons are pressed and enter idle mode
            // as nothing is changing
            Disp2String("\n\r Nothing is pressed");
            LATBbits.LATB8 = 0;
            Idle();
        }
        
     }
}

// Configure CN Interrupt on RA2
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    // clear user defined flags
    RA2flag = 0;
    RA4flag = 0;
    RB4flag = 0;
    

    IEC1bits.CNIE = 0; //disable CN interrupts to stop debounce effects.
    delay_ms(50);
    IEC1bits.CNIE = 1; //enable CN interrupts after waiting for debounce.
    
    // make delays impossible
    delayFlag = 0;
    

    
    if (PORTAbits.RA2 == 0) {
        // set user defined RA2 flag to 1 to signal that it is being pressed
        
        RA2flag = 1;
    } 
    if (PORTAbits.RA4 == 0) {
        
        // set user defined RA4 flag to 1 to signal that it is being pressed
        RA4flag = 1;
    } 
    if (PORTBbits.RB4 == 0) {
        
        // set user defined RB4 flag to 1 to signal that it is being pressed
        RB4flag = 1;
    }
    
    if(RA2flag + RA4flag + RB4flag >= 1){
    
        displayFlag = 1;
    }

    // clear IF flag
    IFS1bits.CNIF = 0;
}

