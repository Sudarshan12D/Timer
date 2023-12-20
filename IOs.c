#include "IOs.h"
#include "TimeDelay.h"
#include "xc.h"
#include "UART2.h"
#include "Alarm.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>

extern int RA2flag;
extern int RA4flag;
extern int RB4flag;
extern int delayFlag;

int minutes = 0;
int seconds = 0;
int held;
int isRunning = 0;


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
        
        if (RA2flag == 1 && !isRunning) {
            
            minutes++;
            if(minutes > 59){

                minutes = 0;
            }
            
            
            displayAlarm(minutes, seconds);
            delay_ms(250);
        }
        else if (RA4flag == 1 && !isRunning) {
            // sets delay for delayFunct to 2 sec  and turns led on and off
            
            
            seconds++;
            if(seconds > 59){

                seconds = 0;
                if(minutes == 59){

                minutes = 0;
                }
                else{
                    minutes++;
                }
            }
            displayAlarm(minutes, seconds);
            delay_ms(250);
        }
        
        else if (RB4flag == 1) {
            held = 1;
            delay_ms(3000);
            if (held) {
                minutes = 0;
                seconds = 0;
                isRunning = 0;
                displayAlarm(minutes, seconds);
                LATBbits.LATB8 = 0;
                Idle();
            } else {
                isRunning = !isRunning;
                LATBbits.LATB8 = 0;
            }
        }
        else if (RA2flag == 0 && RA4flag == 0 && RB4flag == 0) {
            // turns the LED off if no buttons are pressed and enter idle mode
            // as nothing is changing
            
            int done = 0;
            
            if (isRunning) {
                seconds--;
                if (seconds == 0 && minutes <= 0) {
                    done = 1;
                }
                else if (seconds < 0 && minutes > 0) {
                    minutes--;
                    seconds = 59;
                }
                else if (seconds < 0 && minutes <= 0) {
                    done = 1;
                    seconds = 0;
                    minutes = 0;
                }
                displayAlarm(minutes, seconds);
                
                if (done){
                    isRunning = 0;
                    LATBbits.LATB8 = 1;
                    continue;
                }
                
                LATBbits.LATB8 = !LATBbits.LATB8;
                delay_ms(925);
                
            } else {
                Idle();
            }
        }
        
    }
}

// Configure CN Interrupt on RA2
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    // clear user defined flags
    RA2flag = 0;
    RA4flag = 0;
    RB4flag = 0;
    held = 0;
    

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
    
    

    // clear IF flag
    IFS1bits.CNIF = 0;
}

