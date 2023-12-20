
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  


#ifdef	__cplusplus
extern "C" {
#endif 

    //void set_minutes(uint16_t min, uint16_t sec);
    void displayAlarm(uint16_t min, uint16_t sec);
#ifdef	__cplusplus
}
#endif 

#endif	
