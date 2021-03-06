/**
 * @brief       functions.c
 * @details     Functions.
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        01/October/2019
 * @version     01/October/2019   The ORIGIN
 * @pre         N/A
 * @warning     N/A
 */


#include "functions.h"

/**
 * @brief       void Conf_GPIO ( void )
 * @details     It configures GPIO to work with the LEDs.
 *
 *					LEDs:
 * 						- LD1:	PB_5
 * 						- LD2:	PA_5
 * 						- LD3:	PB_6
 * 						- LD4:	PB_7
 *
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        01/October/2019
 * @version		01/October/2019   The ORIGIN
 * @pre         N/A
 * @warning     N/A
 */
void Conf_GPIO ( void )
{
	/* GPIOA and GPIOB Periph clocks enable	 */
	RCC->IOPENR	|=	 ( RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN );

	/* GPIOB Mode: General purpose output mode	 */
	GPIOB->MODER	&=	~( GPIO_MODER_MODE5 | GPIO_MODER_MODE6 | GPIO_MODER_MODE7 );
	GPIOB->MODER	|=	 ( GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0 );

	/* GPIOB Output type: Output push-pull	 */
	GPIOB->OTYPER	&=	~( GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 );

	/* GPIOB Output speed: Low speed	 */
	GPIOB->OSPEEDR	&=	~( GPIO_OSPEEDER_OSPEED5 | GPIO_OSPEEDER_OSPEED6 | GPIO_OSPEEDER_OSPEED7 );

	/* GPIOB Output: No pull-up, pull-down	 */
	GPIOB->PUPDR	&=	~( GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7 );

	/* GPIOB Output: PA_5, PA_6 and PA_7 reset	 */
	GPIOB->BSRR	|=	 ( GPIO_BSRR_BR_5 | GPIO_BSRR_BR_6 | GPIO_BSRR_BR_7 );

	/* GPIOA Mode: General purpose output mode	 */
	GPIOA->MODER	&=	~( GPIO_MODER_MODE5 );
	GPIOA->MODER	|=	 GPIO_MODER_MODE5_0;

	/* GPIOA Output type: Output push-pull	 */
	GPIOA->OTYPER	&=	~GPIO_OTYPER_OT_5;

	/* GPIOA Output speed: Low speed	 */
	GPIOA->OSPEEDR	&=	~GPIO_OSPEEDER_OSPEED5;

	/* GPIOA Output: No pull-up, pull-down	 */
	GPIOA->PUPDR	&=	~GPIO_PUPDR_PUPD5;

	/* GPIOA Output: PB_6 reset	 */
	GPIOA->BSRR	|=	 GPIO_BSRR_BR_5;
}



/**
 * @brief       void Conf_TimerTIM2 ( uint32_t )
 * @details     It configures timer TIM2.
 *
 *				-TIM2:
 * 					-- f_TIM2 = myCLK / ( PSC + 1 ) = 2.097MHz / ( 999 + 1 ) = 2.097 kHz
 * 					-- Interrupt ENABLED.
 * 					-- Overflow: Every 1 second ( ARR / f_TIM2 ) = ( 2097 / 2097 ) = 1
 * 						--- Downcounter.
 * 						--- Prescaler = 1000 - 1 = 999.
 * 						--- ARR = 2097.
 *
 * @param[in]    myCLK:	Timer TIM2 clock.
 *
 * @param[out]   N/A.
 *
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        01/October/2019
 * @version		01/October/2019   The ORIGIN
 * @pre         N/A
 * @warning     N/A
 */
void Conf_TimerTIM2 ( uint32_t myCLK )
{
	/* Timer TIM2 clock enable	 */
	RCC->APB1ENR	|=	 RCC_APB1ENR_TIM2EN;

	/* Timer TIM2:
	 * 	- Disable timer TIM2
	 * 	- Clock division: t_DTS = t_CK_INT
	 * 	- Edge-aligned mode
	 * 	- Counter is not stopped at update event
	 * 	- UEV enabled
	 */
	TIM2->CR1	&=	~( TIM_CR1_CEN | TIM_CR1_CKD | TIM_CR1_CMS | TIM_CR1_DIR | TIM_CR1_OPM | TIM_CR1_UDIS );

	/* Timer TIM2:
	 *  - Master mode
	 */
	TIM2->SMCR	&=	~( TIM_SMCR_SMS | TIM_SMCR_TS );

	/* Reset counter	 */
	TIM2->CNT	 =	 (uint16_t)0U;
	TIM2->PSC	 =	 (uint16_t)( 1000U - 1U );									// Prescaler = 999
	TIM2->ARR	 =	 (uint16_t)( 1 * ( myCLK / ( TIM2->PSC + 1U ) ) + 0.5 );	// Overflow every ~ 1s: f_Timer TIM2: myCLK / ( PSC + 1 ) = 2.097MHz / ( 999 + 1 ) = 2.097 kHz )

	/* Clear Update interrupt flag	 */
	TIM2->SR	&=	~( TIM_SR_UIF );

	/* Enable Interrupt	 */
	NVIC_SetPriority ( TIM2_IRQn, 1 ); 								// Set Priority to 1
	NVIC_EnableIRQ   ( TIM2_IRQn );  								// Enable TIM2_IRQn interrupt in NVIC

	/* Update interrupt enable	 */
	TIM2->DIER	|=	 ( TIM_DIER_UIE );

	/* Timer TIM2:
	 * 	- Auto-reload preload enable
	 * 	- Only counter overflow/underflow generates an update interrupt
	 * 	- Counter used as downcounter
	 */
	TIM2->CR1	|=	 ( TIM_CR1_ARPE | TIM_CR1_URS | TIM_CR1_DIR );
}



/**
 * @brief       void Conf_TimerTIM6 ( uint32_t )
 * @details     It configures basic timer TIM6.
 *
 *				-TIM6:
 * 					-- f_TIM6 = myCLK / ( PSC + 1 ) = 2.097MHz / ( 1998 + 1 ) ~ 1.049 kHz
 * 					-- Interrupt ENABLED.
 * 					-- Overflow: Every 2 second ( ARR / f_TIM6 ) = ( 2098 / 1049 ) ~ 2s
 * 						--- Prescaler = 1999 - 1 = 1998.
 * 						--- ARR = 2098.
 *
 * @param[in]    myCLK:	Timer TIM6 clock.
 *
 * @param[out]   N/A.
 *
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        02/October/2019
 * @version		02/October/2019   The ORIGIN
 * @pre         N/A
 * @warning     N/A
 */
void Conf_TimerTIM6 ( uint32_t myCLK )
{
	/* Timer TIM6 clock enable	 */
	RCC->APB1ENR	|=	 RCC_APB1ENR_TIM6EN;

	/* Timer TIM6:
	 * 	- Disable timer TIM6
	 * 	- Counter is not stopped at update event
	 * 	- UEV enabled
	 */
	TIM6->CR1	&=	~( TIM_CR1_CEN | TIM_CR1_OPM | TIM_CR1_UDIS );

	/* Reset counter	 */
	TIM6->CNT	 =	 (uint16_t)0U;
	TIM6->PSC	 =	 (uint16_t)( 1999 - 1U );									// Prescaler = 1998
	TIM6->ARR	 =	 (uint16_t)( 2 * ( myCLK / ( TIM6->PSC + 1U ) ) + 0.5 );	// Overflow every ~ 2s: f_Timer TIM6: myCLK / ( PSC + 1 ) = 2.097MHz / ( 1998 + 1 ) = 1.049 kHz )

	/* Clear Update interrupt flag	 */
	TIM6->SR	&=	~( TIM_SR_UIF );

	/* Enable Interrupt	 */
	NVIC_SetPriority ( TIM6_IRQn, 1 ); 								// Set Priority to 1
	NVIC_EnableIRQ   ( TIM6_IRQn );  								// Enable TIM6_IRQn interrupt in NVIC

	/* Update interrupt enable	 */
	TIM6->DIER	|=	 ( TIM_DIER_UIE );

	/* Timer TIM6:
	 * 	- Auto-reload preload enable
	 * 	- Only counter overflow/underflow generates an update interrupt
	 */
	TIM6->CR1	|=	 ( TIM_CR1_ARPE | TIM_CR1_URS );
}



/**
 * @brief       void Conf_TimerTIM7 ( uint32_t )
 * @details     It configures basic timer TIM7.
 *
 *				-TIM7:
 * 					-- f_TIM7 = myCLK / ( PSC + 1 ) = 2.097MHz / ( 999 + 1 ) ~ 2.097 kHz
 * 					-- Interrupt DISABLED.
 * 					-- Overflow: Every 0.5 second ( ARR / f_TIM6 ) = ( 1049 / 2097 ) ~ 0.5s
 * 						--- Prescaler = 1000 - 1 = 999.
 * 						--- ARR = 1049.
 *
 * @param[in]    myCLK:	Timer TIM7 clock.
 *
 * @param[out]   N/A.
 *
 *
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        03/October/2019
 * @version		03/October/2019   The ORIGIN
 * @pre         N/A
 * @warning     N/A
 */
void Conf_TimerTIM7 ( uint32_t myCLK )
{
	/* Timer TIM7 clock enable	 */
	RCC->APB1ENR	|=	 RCC_APB1ENR_TIM7EN;

	/* Timer TIM7:
	 * 	- Disable timer TIM7
	 * 	- Counter is not stopped at update event
	 * 	- UEV enabled
	 */
	TIM7->CR1	&=	~( TIM_CR1_CEN | TIM_CR1_OPM | TIM_CR1_UDIS );

	/* Reset counter	 */
	TIM7->CNT	 =	 (uint16_t)0U;
	TIM7->PSC	 =	 (uint16_t)( 1000 - 1U );									// Prescaler = 999
	TIM7->ARR	 =	 (uint16_t)( 0.5 * ( myCLK / ( TIM7->PSC + 1U ) ) + 0.5 );	// Overflow every ~ 0.5s: f_Timer TIM7: myCLK / ( PSC + 1 ) = 2.097MHz / ( 999 + 1 ) = 2.097 kHz )

	/* Clear Update interrupt flag	 */
	TIM7->SR	&=	~( TIM_SR_UIF );

	/* Update interrupt disabled	 */
	TIM7->DIER	&=	~( TIM_DIER_UIE );

	/* Timer TIM7:
	 * 	- Auto-reload preload enable
	 * 	- Only counter overflow/underflow generates an update interrupt
	 */
	TIM7->CR1	|=	 ( TIM_CR1_ARPE | TIM_CR1_URS );
}
