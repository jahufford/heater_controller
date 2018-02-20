/*
 * settings_dstate.cpp
 *
 *  Created on: Feb 18, 2018
 *      Author: joe
 */


#include "settings_dstate.h"
#include "WM.h"
#include "display.h"
#include "globals.h"
#include "wireless_uart.h"

#define WIDGET_ID_BUTTON (GUI_ID_USER + 0)

SettingsDState::SettingsDState(DisplayStates previous_state) : DisplayState(previous_state)
{
	button= BUTTON_CreateEx(95,75,110,70,NULL, WM_CF_SHOW,0, WIDGET_ID_BUTTON);
    BUTTON_SetText(button, "Hello");
    BUTTON_SetFont(button,GUI_FONT_COMIC24B_1);
    old_cb = WM_SetCallback(button, &Display::DisplayCallback);
    display_dirty = true;
    GUI_SetFont(&GUI_Font8x16);
    GUI_GotoXY(0,0);
    GUI_DispString("Hello world!");

    GUI_SetColor(GUI_WHITE);
    GUI_DrawRect(0,0,319,239);
}
SettingsDState::~SettingsDState()
{
	asm("nop");
}
void SettingsDState::Paint()
{

   // while(1){

    if(display_dirty){
    	//vTaskDelay(pdMS_TO_TICKS(500));
        GUI_DispDecAt(current_temp,50,100,3);
        GUI_DispCharAt(sent_char,50,120);
        GUI_DispCharAt(recv_char,50,140);
    	display_dirty = false;
    }
    WM_Exec();
    GUI_Exec();
}
void SettingsDState::HandleEvents(WM_MESSAGE * pMsg)
{
	asm("nop");
	if(pMsg == NULL)
	{ // non STemWin events
	    if(xQueueReceive(temp_queue,&current_temp,0) != errQUEUE_EMPTY)
        {
	    	display_dirty = true;
	    }
        if(xQueueReceive(sent_queue,&sent_char,0) != errQUEUE_EMPTY)
        {
	    	display_dirty = true;
        }
        if(xQueueReceive(wireless_queue,&recv_char,0) != errQUEUE_EMPTY)
        {
	    	display_dirty = true;
        }
	}else{
      // STemWin events
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
        display_dirty = true;
	}
	asm("nop");
}
void SettingsDState::DoLogic(TickType_t ticks_elapsed)
{
	//auto ms_elapsed = pdTICK_TO_MS(ticks_elapsed);
	volatile uint32_t ticks_per_ms = portTICK_PERIOD_MS;
	asm("nop");
}
