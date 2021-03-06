/**
 * @brief       main.c
 * @details     It changes the state of the LEDs using the Timers.
 *
 *                  - LED1. Timer0: 32-bit, Channel 0, Interrupt ON.    Blinks every 0.125s.
 *                  - LED2. Timer1: 16-bit, Channel 0, Interrupt OFF.   Blinks every 0.25s.
 *                  - LED3. Timer2:  8-bit, Channel 0, Interrupt ON.    Blinks every 0.375s.
 *                  - LED4. Timer0: 32-bit, Channel 1, Interrupt ON.    Blinks every 0.5s.
 *
 *              This firmware is just an example about how to use the timers on the NRF52832. On
 *              polling mode or by interrupts with different channels.
 *
 *
 * @return      N/A
 *
 * @author      Manuel Caballero
 * @date        14/February/2018
 * @version     14/February/2018    The ORIGIN
 * @pre         This firmware was tested on the nrf52-DK with Segger Embedded Studio v3.34
 *              ( SDK 14.2.0 ).
 * @warning     The softdevice (s132) is taken into account, Bluetooth was not used although.
 * @pre         This code belongs to AqueronteBlog ( http://unbarquero.blogspot.com ).
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nordic_common.h"
#include "nrf.h"
#include "variables.h"
#include "functions.h"
#include "interrupts.h"



/**@brief Function for application main entry.
 */
int main(void)
{
  conf_GPIO   ();
  conf_TIMER0 ();
  conf_TIMER1 ();
  conf_TIMER2 ();

  NRF_TIMER0->TASKS_START = 1;    // Start Timer0
  NRF_TIMER2->TASKS_START = 1;    // Start Timer2

  while( 1 )
  {
      NRF_P0->OUTSET        =   ( 1 << LED2 );
      delay();
      NRF_P0->OUTCLR        =   ( 1 << LED2 );
      delay();

      /*
      //NRF_POWER->SYSTEMOFF = 1;
      NRF_POWER->TASKS_LOWPWR = 1;        // Sub power mode: Low power.
      // Enter System ON sleep mode
      __WFE();
      // Make sure any pending events are cleared
      __SEV();
      __WFE();
      */
    }
}


/**
 * @}
 */
