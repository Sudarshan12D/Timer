#include <p24F16KA101.h>

#include "TimeDelay.h"
#include "ChangeClk.h"
#include "xc.h"
#include <p24fxxxx.h>

extern int delayFlag;

// funct that creates a delay of length: delay seconds
void delay_ms(int time_ms) {
    
    // if no delay is desired return from function without starting timer
    if (delayFlag == 0) {
        return;
    }
    
    // set default prescaler for 1, 2, or 3 second delay
    int prescaler = 0x3;          // 1:256
    uint16_t PR2_int =  time_ms / 16;
    
    NewClk(32);      // 32kHz
    
    // Setting up timer
    T2CONbits.TSIDL = 0;         // contiue in idle mode
    T2CONbits.T32 = 0;           // use timer 2 and timer 3 seperately
    T2CONbits.TCS = 0;           // use internal clock
    T2CONbits.TCKPS = prescaler; // set prescaler
    
    // Setting up timer interrupts
    if (IFS1bits.CNIF == 0) {
            IPC1bits.T2IP = 1; // set priority as lowest for regular delay
    } else {
        IPC1bits.T2IP = 7; // set priority as highest for debounce delay
    }
    IFS0bits.T2IF = 0; // clear timer 2 flag
    IEC0bits.T2IE = 1; // enable timer 2 interrupt
    
    // Start timer
    TMR2 = 0;          // Clear timer bits
    PR2 = PR2_int;     // Set PR2
    T2CONbits.TON = 1; // Start timer
    
    Idle();
    
    NewClk(8);
    
    return;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0; // Clear timer 2 interrupt flag
    TMR2 = 0;
    T2CONbits.TON = 0; // stop timer
    return;
}
