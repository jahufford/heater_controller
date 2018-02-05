/*
 * main_screen.cpp
 *
 *  Created on: Feb 4, 2018
 *      Author: joe
 */

#include "main_screen.h"
#include "WM.h"
#include "display.h"
#include "globals.h"
#include "wireless_uart.h"

#define WIDGET_ID_BUTTON (GUI_ID_USER + 0)

MainScreen::MainScreen()
{
	button= BUTTON_CreateEx(95,75,110,70,NULL, WM_CF_SHOW,0, WIDGET_ID_BUTTON);
    BUTTON_SetText(button, "Hello");
    BUTTON_SetFont(button,GUI_FONT_COMIC24B_1);
    old_cb = WM_SetCallback(button, &Display::DisplayCallback);
}
MainScreen::~MainScreen()
{
	asm("nop");
}
void MainScreen::Paint()
{
    GUI_SetFont(&GUI_Font8x16);
    GUI_DispString("Hello world!");

    GUI_SetColor(GUI_WHITE);
    GUI_DrawRect(0,0,319,239);

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
    	WM_Exec();
    	GUI_Exec();
    	//vTaskDelay(pdMS_TO_TICKS(500));
    }
}
void MainScreen::HandleEvents(WM_MESSAGE * pMsg)
{
	asm("nop");
	uint32_t bu = (uint32_t)button;
	if(pMsg->hWin == button)
	{

        switch(pMsg->MsgId)
        {
        default:
        	old_cb(pMsg);
            WM_DefaultProc(pMsg);
        }
	}else{
      WM_DefaultProc(pMsg);
	}
	asm("nop");
}
