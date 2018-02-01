/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#ifdef USE_RTOS_SYSTICK
#include <cmsis_os.h>
#endif
#include "stm32f4xx_it.h"
#include "touchscreen.h"
#include "GUI.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "wireless_uart.h"

extern TIM_HandleTypeDef	htim8;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles SysTick Handler, but only if no RTOS defines it.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	//HAL_SYSTICK_IRQHandler();
	OS_TimeMS++; // for the GUI
#ifdef USE_RTOS_SYSTICK
	osSystickHandler();
#endif
}

void NMI_Handler()
{
	while(1){
		asm("nop");
	}
}

void HardFault_Handler()
{
	//while(1){
		asm("nop");
	//};
}

void MemManage_Handler()
{
	while(1){
		asm("nop");
	}
}

void UsageFault_Handler()
{
	while(1){
		asm("nop");
	}
}


//void SVC_Handler()
//{
//	while(1){
//		asm("nop");
//	}
//}

void DebugMon_Handler()
{
	while(1){
		asm("nop");
	}
}

//void PendSV_Handler()
//{
//	while(1){
//		asm("nop");
//	}
//}
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

void EXTI9_5_IRQHandler(void)
{
	// handle touchscreen interrupt
	if(__HAL_GPIO_EXTI_GET_IT(TOUCHSCREEN_IRQ_PIN)){
      EXTI->IMR &= ~(1<<7); // mask the interrupt
      if(touchscreen_is_pressed){
    	  EXTI->RTSR &= ~(TOUCHSCREEN_IRQ_PIN); // turn off rising edge
    	  EXTI->FTSR |= TOUCHSCREEN_IRQ_PIN; // turn on falling edge
          touchscreen_is_pressed = 0;
          GUI_PID_STATE pstate;
          pstate.Pressed = 0;
          pstate.Layer = 0;
          GUI_PID_StoreState(&pstate);
          HAL_TIM_Base_Stop(&h_touchpressed_ticker);
      }else{
    	  EXTI->FTSR &= ~(TOUCHSCREEN_IRQ_PIN); // turn off falling edge
    	  EXTI->RTSR |= (TOUCHSCREEN_IRQ_PIN);
          touchscreen_is_pressed = 1;
          // TODO
          // read x y coordinates, and insert to PID queue of the window manager
          TS_StartRead();
          int x = TS_GetX(3);//  average a few reads, since quick taps tend to make a bogus read initially
          int y = TS_GetY(3);
          TS_SetIdle();
          TS_EndRead();
          GUI_PID_STATE pstate;
          pstate.x = x;
          pstate.y = y;
          pstate.Pressed = 1;
          pstate.Layer = 0;
          GUI_PID_StoreState(&pstate);
          HAL_TIM_Base_Start(&h_touchpressed_ticker);
        }
      EXTI->IMR |= (1<<7); // unmask the interrupt
      __HAL_GPIO_EXTI_CLEAR_IT(TOUCHSCREEN_IRQ_PIN);
	}
}
void TIM5_IRQHandler(void)
{
    asm("nop");
    TS_StartRead();
    int x = TS_GetX(3);//  average a few reads, since quick taps tend to make a bogus read initially
    int y = TS_GetY(3);
    TS_SetIdle();
    TS_EndRead();
    GUI_PID_STATE pstate;
    pstate.x = x;
    pstate.y = y;
    pstate.Pressed = 1;
    pstate.Layer = 0;
    GUI_PID_StoreState(&pstate);
    TIM5->SR &= ~TIM_SR_UIF;
}
void EXTI15_10_IRQHandler(void)
{
	asm("nop");
//	char var = 'A';
//	volatile uint32_t reg;
//    reg = EXTI->PR;
//    uint32_t rreg;
//    rreg = GPIO_PIN_13;
//    asm("nop");
//    EXTI->PR |= reg;
    EXTI->PR |= EXTI_PR_PR13;
//    EXTI->IMR &= ~(1<<13);
//    uint8_t byte[2];
//	TS_WriteData(0x90); // back to idle
//	if(HAL_SPI_Receive(&h_touchscreen_spi,(uint8_t*)byte, 1,0xFFFF) != HAL_OK){
//	   Error_Handler();
//	}
//	if(HAL_SPI_Receive(&h_touchscreen_spi,(uint8_t*)&byte[1], 1,0xFFFF) != HAL_OK){
//	   Error_Handler();
//	}
//    reg = EXTI->PR;
    //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    //HAL_UART_Transmit(&UartHandleWireless, (uint8_t*)&var,1,0xFFFF);
}

void USART2_IRQHandler(void)
{
	uint32_t reg;
	reg = USART2->SR;

	volatile uint8_t data;
	data = USART2->DR;
	//has_new_char = 1;
	//received_char = data;
//	  NVIC_ClearPendingIRQ(USART2_IRQn);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void USART1_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken =pdFALSE;
	uint32_t reg;
	reg = USART1->SR;

	volatile uint8_t data;
	data = USART1->DR;
	xQueueSendToBackFromISR(wireless_queue,&data,&xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//  The above line is equivalent to
//	if(xHigherPriorityTaskWoken){
//		taskYIELD();
//	}
	//has_new_char = 1;
	//received_char = data;
	asm("nop");
}
void TIM8_BRK_TIM12_IRQHandler(void){
	asm("nop");
}
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	asm("nop");
}
void TIM8_CC_IRQHandler(void)
{
	asm("nop");
}
void TIM8_UP_TIM13_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 0 */

  /* USER CODE END TIM8_UP_TIM13_IRQn 0 */
   HAL_IncTick();
   HAL_TIM_IRQHandler(&htim8);
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 1 */

  /* USER CODE END TIM8_UP_TIM13_IRQn 1 */
}
