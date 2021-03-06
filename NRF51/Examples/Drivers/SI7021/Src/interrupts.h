/**
 * @brief       interrupts.h
 * @details     Interrupts header.
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        5/February/2018
 * @version     5/February/2018    The ORIGIN
 * @pre         N/A
 * @warning     N/A
 * @pre         This code belongs to AqueronteBlog ( http://unbarquero.blogspot.com ).
 */

#include "variables.h"


/* INTERRUPT PRTOTYPES */
void TIMER0_IRQHandler ( void );



/* VARIABLES */
extern uint8_t   dataToBeTX;
extern uint32_t  mySTATE;
extern uint32_t  TX_inProgress;
extern uint8_t   TX_buff[];
extern uint8_t   *myPtr;
