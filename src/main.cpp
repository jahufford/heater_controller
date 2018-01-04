/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"
#include "task.h"
#include "debug_serial_port.h"
#include "temperature.h"
#include "wireless_uart.h"
#include "globals.h"
#include "display.h"
#include "error.h"
#include <stdio.h>
#include "GUI.h"
#include "BUTTON.h"
#include "WM.h"
#include <functional>

void CRC_Init();
void Nucleo_Button_Init(void);
void SystemClock_Config(void);
void Nucleo_Led_Init(void);


#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

void HeartbeatTask(void *ptr)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	while(1){
		printf("Hello Heater Controller World\n");
		vTaskDelay(pdMS_TO_TICKS(1000)); // task delay takes ticks, not milliseconds
		//xTicksToDelay(pdMS_TO_TICKS(1000));
		//vTaskDelayUntil(&xLastWakeTime, 1000);
	}
}

#include <stddef.h>
#include <string.h>
#include "WM.h"
#include "FRAMEWIN.h"
#include "BUTTON.h"



/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Recommended memory to run the sample with adequate performance
//
#define RECOMMENDED_MEMORY (1024L * 5)

///*********************************************************************
//*
//*       Static data
//*
//**********************************************************************
//*/
//static int _Color;
//static int _Font;
//static int _Pressed;
//
//static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
//  { FRAMEWIN_CreateIndirect, "Round button sample", 0,      50,  60, 200, 120, FRAMEWIN_CF_MOVEABLE },
//  { BUTTON_CreateIndirect,   "Button",   GUI_ID_BUTTON0,   100,  10,  80,  80 },
//  { BUTTON_CreateIndirect,   "Callback", GUI_ID_BUTTON1,    10,  10,  60,  20 },
//  { BUTTON_CreateIndirect,   "Font",     GUI_ID_BUTTON2,    10,  30,  60,  20 },
//  { BUTTON_CreateIndirect,   "Color",    GUI_ID_BUTTON3,    10,  50,  60,  20 },
//  { BUTTON_CreateIndirect,   "Cancel",   GUI_ID_CANCEL,     10,  70,  60,  20 }
//};
//
///*********************************************************************
//*
//*       Static functions
//*
//**********************************************************************
//*/
///*********************************************************************
//*
//*       _OnPaint
//*
//* Function description
//*   Paints the owner drawn button
//*/
//static void _OnPaint(BUTTON_Handle hObj) {
//  int Index;
//  char ac[50];
//  GUI_RECT Rect;
//
//  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
//  WM_GetClientRect(&Rect);
//  //
//  // Draw filled ellipse with button background color
//  //
//  GUI_SetColor(BUTTON_GetBkColor(hObj, Index));
//  GUI_FillEllipse(Rect.x1 / 2, Rect.y1 / 2, Rect.x1 / 2, Rect.y1 / 2);
//  //
//  // Draw black shape
//  //
//  GUI_SetColor(GUI_BLACK);
//  GUI_DrawEllipse(Rect.x1 / 2, Rect.y1 / 2, Rect.x1 / 2, Rect.y1 / 2);
//  //
//  // Draw button text with widget attributes
//  //
//  GUI_SetColor(BUTTON_GetTextColor(hObj, Index));
//  GUI_SetBkColor(BUTTON_GetBkColor(hObj, Index));
//  GUI_SetFont(BUTTON_GetFont(hObj));
//  BUTTON_GetText(hObj, ac, sizeof(ac));
//  if (_Pressed) {
//    strcpy(ac + strlen(ac), "\npressed");
//  }
//  GUI_DispStringInRect(ac, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
//}
//
///*********************************************************************
//*
//*       _cbButton
//*
//* Function description
//*  1. Calls the owner draw function if the WM_PAINT message has been send
//*  2. Calls the original callback for further messages
//*  3. After processing the messages the function evaluates the pressed-state
//*     if the WM_TOUCH message has been send
//*/
//static void _cbButton(WM_MESSAGE * pMsg) {
//  switch (pMsg->MsgId) {
//    case WM_PAINT:
//      _OnPaint(pMsg->hWin);
//      break;
//    default:
//      BUTTON_Callback(pMsg); // The original callback
//      break;
//  }
//  if (pMsg->MsgId == WM_TOUCH) {
//    if (BUTTON_IsPressed(pMsg->hWin)) {
//      if (!_Pressed) {
//        _Pressed = 1;
//      }
//    } else {
//      _Pressed = 0;
//    }
//  }
//}
//
///*********************************************************************
//*
//*       _cbDialog
//*
//* Function description
//*   Dialog callback routine
//*/
//static void _cbDialog(WM_MESSAGE * pMsg) {
//  int           NCode;
//  int           Id;
//  WM_HWIN       hDlg;
//  BUTTON_Handle hButton;
//
//  hDlg = pMsg->hWin;
//  switch (pMsg->MsgId) {
//    case WM_PAINT:
//      WM_DefaultProc(pMsg); // Handle dialog items
//      //
//      // After drawing the dialog items add some user drawn items to the window
//      //
//      GUI_SetPenSize(10);
//      GUI_SetColor(GUI_GREEN);
//      GUI_DrawLine( 95,  5, 185, 95);
//      GUI_SetColor(GUI_RED);
//      GUI_DrawLine( 95, 95, 185,  5);
//      break;
//    case WM_INIT_DIALOG:
//      hButton = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
//      WM_SetHasTrans(hButton);              // Set transparency flag for button
//      break;
//    case WM_KEY:
//      switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
//        case GUI_KEY_ESCAPE:
//          GUI_EndDialog(hDlg, 1);
//          break;
//        case GUI_KEY_ENTER:
//          GUI_EndDialog(hDlg, 0);
//          break;
//      }
//      break;
//    case WM_NOTIFY_PARENT:
//      Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
//      NCode = pMsg->Data.v;                 // Notification code
//      switch (NCode) {
//        case WM_NOTIFICATION_RELEASED:      // React only if released
//          hButton = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
//          if (Id == GUI_ID_BUTTON1) {       // Toggle callback
//            if (WM_GetCallback(hButton) == _cbButton) {
//              WM_SetCallback(hButton, BUTTON_Callback);
//            } else {
//              WM_SetCallback(hButton, _cbButton);
//            }
//            WM_InvalidateWindow(hButton);
//          }
//          if (Id == GUI_ID_BUTTON2) {       // Toggle font
//            if (_Font) {
//              BUTTON_SetFont(hButton, &GUI_Font13_1);
//            } else {
//              BUTTON_SetFont(hButton, &GUI_Font8x16);
//            }
//            _Font ^= 1;
//          }
//          if (Id == GUI_ID_BUTTON3) {       // Toggle color
//            if (_Color) {
//              BUTTON_SetBkColor(hButton, 0, 0xaaaaaa);
//              BUTTON_SetBkColor(hButton, 1, GUI_WHITE);
//              BUTTON_SetTextColor(hButton, 0, GUI_BLACK);
//              BUTTON_SetTextColor(hButton, 1, GUI_BLACK);
//            } else {
//              BUTTON_SetBkColor(hButton, 0, GUI_BLUE);
//              BUTTON_SetBkColor(hButton, 1, GUI_RED);
//              BUTTON_SetTextColor(hButton, 0, GUI_WHITE);
//              BUTTON_SetTextColor(hButton, 1, GUI_YELLOW);
//            }
//            _Color ^= 1;
//          }
//          if (Id == GUI_ID_OK) {            // OK Button
//            GUI_EndDialog(hDlg, 0);
//          }
//          if (Id == GUI_ID_CANCEL) {        // Cancel Button
//            GUI_EndDialog(hDlg, 1);
//          }
//          break;
//      }
//      break;
//    default:
//      WM_DefaultProc(pMsg);
//  }
//}
//
///*********************************************************************
//*
//*       Public code
//*
//**********************************************************************
//*/
///*********************************************************************
//*
//*       MainTask
//*/
//void GUITask(void *ptr) {
//  GUI_Init();
//  //
//  // Check if recommended memory for the sample is available
//  //
//  if (GUI_ALLOC_GetNumFreeBytes() < RECOMMENDED_MEMORY) {
//    GUI_ErrorOut("Not enough memory available.");
//    return;
//  }
//  //
//  // Use memory devices for all windows
//  //
//  #if GUI_SUPPORT_MEMDEV
//    WM_SetCreateFlags(WM_CF_MEMDEV);
//    WM_EnableMemdev(WM_HBKWIN);
//  #endif
//  WM_SetDesktopColor(GUI_GREEN);
//  while(1) {
//    _Font = _Color = 0;
//    GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, 0, 0, 0);
//    taskYIELD();
//    vTaskDelay(1000);
//  }
//}

void GUITask(void* ptr)
{
	GUI_Init();

    GUI_SetFont(&GUI_Font8x16);
    GUI_DispString("Hello world!");
    GUI_DispDecAt( 27, 20,20,4);


//    volatile int x = GUI_IsInitialized();
    GUI_SetBkColor(GUI_DARKRED);
    GUI_Clear();
    asm("nop");
    //GUI_FillRect(20,20,280,150);
    //GUI_SetBkColor(GUI_DARKRED);
    //GUI_SetColor(LCD_MakeColor(GUI_BLUE));
    GUI_SetColor(GUI_MAKE_COLOR(GUI_RED));
    uint32_t colors[25] = {GUI_BLUE,GUI_GREEN,GUI_RED, GUI_CYAN,GUI_MAGENTA,GUI_YELLOW,
    					   GUI_LIGHTBLUE,GUI_LIGHTGREEN,GUI_LIGHTRED,GUI_LIGHTCYAN, GUI_LIGHTMAGENTA,
						   GUI_LIGHTYELLOW,GUI_DARKBLUE,GUI_DARKGREEN,GUI_DARKRED,GUI_DARKCYAN,
						   GUI_DARKMAGENTA,GUI_DARKYELLOW,GUI_WHITE,GUI_LIGHTGRAY,GUI_GRAY,
						   GUI_DARKGRAY,GUI_BLACK,GUI_BROWN,GUI_ORANGE};
    GUI_SetColor(GUI_WHITE);
    GUI_DrawRect(0,0,319,239);
//    uint8_t color_index = 0;
//    for(int i=10;i<310/2;i+=10){
//    	GUI_SetColor(colors[color_index]);
//    	color_index++;
//    	color_index%=25;
//    	GUI_DrawRect(i,i,319-i,239-i);
//    }
//    GUI_SetColor(GUI_RED);
//    GUI_FillRect(20,20,80,40);
//
//    GUI_SetColor(GUI_BLUE);
//    GUI_SetBkColor(GUI_GREEN);
//    GUI_DispCharAt('A',3,200);
//    GUI_DispStringAt("Hi", (LCD_GetXSize()-100)/2, (LCD_GetYSize()-20)/2);
//    GUI_SetBkColor(GUI_CYAN);
//    GUI_DispStringAt("Hello World!", (LCD_GetXSize()-100)/2, 200);
//    GUI_SetColor(GUI_YELLOW);
//    GUI_SetFont(GUI_FONT_COMIC24B_1);
//    GUI_DispStringAt("Hello World!", 95,150);
    GUI_SetColor(GUI_YELLOW);
    GUI_SetBkColor(GUI_BLACK);
//    WM_Exec();
//    GUI_Exec();
//
//    button= BUTTON_CreateEx(95,75,110,70,NULL, WM_CF_SHOW,0, WIDGET_ID_BUTTON);
//    BUTTON_SetText(button, "Hello");
//    BUTTON_SetFont(button,GUI_FONT_COMIC24B_1);
//
//    old_cb = WM_SetCallback(button, MyCallback);
//
//    while(1){
//    	int temp;
//    	if(xQueueReceive(temp_queue,&temp,0) != errQUEUE_EMPTY){
//    		GUI_DispDecAt(temp,50,100,3);
//    	}
//    	char sent_char;
//        if(xQueueReceive(sent_queue,&sent_char,0) != errQUEUE_EMPTY){
//            GUI_DispCharAt(sent_char,50,120);
//        }
//    	char recv_char;
//    	if(xQueueReceive(wireless_queue,&recv_char,0) != errQUEUE_EMPTY){
//    		GUI_DispCharAt(recv_char,50,140);
//    	}
//    	GUI_Exec();
//    	//vTaskDelay(pdMS_TO_TICKS(500));
//    }
}

QueueHandle_t queue1 = xQueueCreate(1,1);
QueueHandle_t queue2 = xQueueCreate(1,1);

// keeps from being preempted by another task. But will still be interrupted by interrupts
auto CriticalSection = [](auto func){
	vTaskSuspendAll();
	func();
	xTaskResumeAll();
};

void vTask1(void *pvParameters){
//auto vTask1 = [](void *pvParameters){
	const char *pcTaskName = "Task 1 is running, received %d \r\n";
	volatile uint32_t ul;
	uint8_t signal;
	uint8_t recv;
	signal = 1;
	for(;;){
		xQueueReceive(queue1,&recv,portMAX_DELAY);
		CriticalSection([&]{ // we're safe to use default reference capture, since this lambda won't outlive the function
				printf(pcTaskName,recv);
		});
		vTaskDelay(1000);
		volatile int ret = xQueueSendToBack(queue2,&signal,10);
		signal++;
//		taskYIELD();
//		for(ul=0;ul < 999900; ul++){
//
//		}
	}
};
void vTask2(void *pvParameters){
	const char *pcTaskName = "Task 2 is running, received %d \r\n";
	volatile uint32_t ul;
	uint8_t signal = 255;
	uint8_t recv;

	for(;;){
		xQueueReceive(queue2,&recv,portMAX_DELAY);
   		CriticalSection([&]{
            printf(pcTaskName,recv);
   		});
		signal--;
		vTaskDelay(1000);
		xQueueSendToBack(queue1,&signal,portMAX_DELAY);

//		for(ul=0;ul < 999900; ul++){
//
//		}
	}
}
int main(void)
{
	HAL_Init();
    SystemClock_Config();

    CRC_Init();

    //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    DebugSerialPort_Init(); // now we can use printf

    Nucleo_Button_Init();

    if(!Temperature_HardwareInit()){
    	Error_Handler();
    }

    if(!WirelessUart_HardwareInit()){
    	Error_Handler();
    }

    temp_queue = xQueueCreate(1,sizeof(int));
    sent_queue = xQueueCreate(1,sizeof(char));

    // stack size is given as number of words NOT bytes
    // higher priority number is higher priority, it's the opposite of arm interrupt priorities
//    if(xTaskCreate(HeartbeatTask, "HeartbeatTask",512,0,2,0) != pdPASS){
//    	//pd stand for Project Defs
//    	//handle error, check heap status
//    }
//    if(xTaskCreate(GUITask, "GuiTask", 512,0,0,0) != pdPASS){ // very low priority
//
//    }
//    auto gui_func = [](void *disp){
//    	auto display = static_cast<Display*>(disp);
//    	display->RunDisplay();
//    };
//    // wire up gui task
//    if(xTaskCreate(gui_func,"Gui Task", 512,static_cast<void*>(&display),0,0) != pdPASS)
//    {
//    	// TODO handle error
//    }
//
//    //queue2 = xQueueCreate(1,1);
//    xTaskCreate(vTask1,"Task 1", 500,NULL,1,NULL);
//    xTaskCreate(vTask2,"Task 2", 500,NULL,1,NULL);
    // create a task with a lambda. Even something simple is ugly
//    xTaskCreate([](auto data){
//    	for(;;){
//    		CriticalSection([]{
//    				printf("you've been labmda'd\n");
//    		});
//    		vTaskDelay(1000);
//    	}
//    },"Lambda Task",500,NULL,1,NULL);
    // better to
//    auto lamb = [](auto data){
//    	for(;;){
//            CriticalSection([]{
//              printf("LAMBDA STAMPED\n");
//            });
//            vTaskDelay(5000);
//    	}
//    };
    volatile BaseType_t ret;
//    ret = xTaskCreate(lamb, "Stamp", 500,NULL,1,NULL);

    auto uart_task = [](void *data){
        char var = 'a';
    	for(;;){
            xQueueSendToBack(sent_queue,&var,10);
    		CriticalSection([var]{
                printf("Sending %c\n", var);
                // the call to HAL_UART_Trasmit needs to be in a critical section so it's not interrupted by a task
                // TODO figuring out the clock thing, does the rtos ticker and hal
                // ticker work during this function call
                HAL_UART_Transmit(&UartHandleWireless, (uint8_t*)&var,1,HAL_MAX_DELAY);
            });
            var++;
            if(var=='z'){
              var = 'a';
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
    	}
    };
    ret = xTaskCreate(uart_task, "uart",500,NULL,1,NULL);

    TimerHandle_t temp_timer = xTimerCreate("Temp Timer", pdMS_TO_TICKS(500),1,0, [](TimerHandle_t xTimer){
    	int temp = Temperature_Read();
    	if(temp != TEMP_ERROR){
    		printf("Temp = %d\n", temp);
    		xQueueSendToBack(temp_queue,&temp,10);
    	}
    });

    xTimerStart(temp_timer,0);

//    TimerHandle_t wireless_timer = xTimerCreate("Wireless Timer",pdMS_TO_TICKS(1000),1,0,[](TimerHandle_t xTimer){
//    	static char var = 'a';
//    	printf("Sending %c\n", var);
//    	//printf("Hello\n");
//    	HAL_UART_Transmit(&UartHandleWireless, (uint8_t*)&var,1,0xFFFF);
//    	var++;
//    	if(var=='z'){
//    		var = 'a';
//    	}
//    });
//    xTimerStart(wireless_timer,0);

//    xTaskCreate(vTask2,"Task 2", 1000,(void*)queue2,1,NULL);
//    xTaskCreate(vTask2,"Task 2", 1000,(void*)queue2,1,NULL);
//    uint8_t signal = 1;
//    xQueueSendToBack(queue1,&signal,portMAX_DELAY);
    vTaskStartScheduler(); // we're using the generic scheduler, since configUSE_PORT_OPTIMISED_TASK_SELECTION is undefined

	for(;;);
}


// set up the user button on the nucleo and enable it's interrupt
void Nucleo_Button_Init(void)
{
   	// set up the button
	GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_13;
   	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
   	GPIO_InitStruct.Pull = GPIO_NOPULL;
   	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

   	 // generate interrupt when button is pressed
    volatile uint32_t reg;
    reg = SYSCFG->EXTICR[3];
    reg &= ~(0xF << 4);
    reg |= (SYSCFG_EXTICR4_EXTI13_PC);
    SYSCFG->EXTICR[3] = reg;
//    EXTI->RTSR &= ~(1<<13);
//    //EXTI->FTSR |= (1<<13);
//    EXTI->FTSR |= EXTI_FTSR_TR13;
//    //EXTI->IMR |= (1<<13);
//    EXTI->IMR |= EXTI_IMR_MR13;
    NVIC_SetPriority(EXTI15_10_IRQn, 1);
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}


void Nucleo_Led_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LD2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);
}
void CRC_Init()
{
	// turn on CRC for STemWin
    __CRC_CLK_ENABLE();
    CRC_HandleTypeDef hcrc;
    hcrc.Instance = CRC;
    if (HAL_CRC_Init(&hcrc) != HAL_OK)
    {
        Error_Handler();
    }
}
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows:
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

   /* Activate the OverDrive to reach the 180 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

}
