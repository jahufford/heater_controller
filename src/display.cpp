/*
 * display.cpp
 *
 *  Created on: Jan 1, 2018
 *      Author: joe
 */

#include "display.h"
#include "stm32f4xx.h"
#include "error.h"
#include "FreeRTOS.h"
#include "globals.h"
#include <functional>

#include "wireless_uart.h"

#define WIDGET_ID_BUTTON (GUI_ID_USER + 0)
WM_CALLBACK *old_cb;


Display display;

void DisplayCallback(WM_MESSAGE * pMsg)
{
	asm("nop");
	uint32_t bu = (uint32_t)display.button;
//	if(pMsg->hWin == this->button)
//	{
//
//        switch(pMsg->MsgId)
//        {
//        default:
//        	old_cb(pMsg);
//            WM_DefaultProc(pMsg);
//        }
//	}else{
//      WM_DefaultProc(pMsg);
//	}
	asm("nop");
}

Display::Display()
{
	// turn on CRC for STemWin
    __CRC_CLK_ENABLE();
    CRC_HandleTypeDef hcrc;
    hcrc.Instance = CRC;
    if (HAL_CRC_Init(&hcrc) != HAL_OK)
    {
        Error_Handler();
    }
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

    button= BUTTON_CreateEx(95,75,110,70,NULL, WM_CF_SHOW,0, WIDGET_ID_BUTTON);
    BUTTON_SetText(button, "Hello");
    BUTTON_SetFont(button,GUI_FONT_COMIC24B_1);

    old_cb = WM_SetCallback(button, &DisplayCallback);

}

void Display::RunDisplay()
{
	Paint();
}
void Display::Paint()
{
	// paint the status bar
	if(show_status_bar)
	{

	}


    while(1){
    	int temp;
    	if(xQueueReceive(temp_queue,&temp,0) != errQUEUE_EMPTY){
    		GUI_DispDecAt(temp,50,100,3);
    	}
    	char sent_char;
        if(xQueueReceive(sent_queue,&sent_char,0) != errQUEUE_EMPTY){
            GUI_DispCharAt(sent_char,50,120);
        }
    	char recv_char;
    	if(xQueueReceive(wireless_queue,&recv_char,0) != errQUEUE_EMPTY){
    		GUI_DispCharAt(recv_char,50,140);
    	}
    	GUI_Exec();
    	//vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void Display::HandleEvents(WM_MESSAGE * pMsg)
{

}

void Display::ShowStatusBar(bool show_status_bar)
{
	this->show_status_bar = show_status_bar;
}
