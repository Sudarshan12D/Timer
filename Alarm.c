/*
 * File:   Alarm.c
 * Author: SUDARSHAN
 *
 * Created on 1 November, 2023, 5:54 PM
 */


#include "xc.h"
#include "Alarm.h"
#include "ChangeClk.h"
#include "UART2.h"

extern int isRunning;

void displayAlarm(uint16_t min, uint16_t sec){
    
    IEC1bits.CNIE = 0; //disable CN interrupts 
    
    NewClk(8);
    Disp2Dec(min);
    Disp2String("m");
    Disp2String(" : ");
    
    Disp2Dec(sec);
    Disp2String("s");
    
    if (isRunning && min == 0 && sec == 0) {
        Disp2String(" -- ALARM");
    } else {
        Disp2String("         ");
    }
    
    Disp2String("\r");
    
    NewClk(32);
    
    IEC1bits.CNIE = 1; //enable CN interrupts 
    
}
