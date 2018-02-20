/*
 * main_screen.cpp
 *
 *  Created on: Feb 4, 2018
 *      Author: joe
 */

#include "main_screen_dstate.h"
#include "WM.h"
#include "display.h"
#include "globals.h"
#include "wireless_uart.h"

#define WIDGET_ID_BUTTON (GUI_ID_USER + 0)

MainScreen::MainScreen(DisplayStates previous_state) : DisplayState(previous_state)
{
	GUI_Clear();
	WM_HWIN desktop_win = WM_GetDesktopWindow();
	WM_SetCallback(desktop_win, &Display::DisplayCallback);
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
MainScreen::~MainScreen()
{
	WM_DeleteWindow(button);
	WM_HWIN desktop_win = WM_GetDesktopWindow();
//	WM_DeleteWindow(desktop_win);
	asm("nop");
}
void MainScreen::Paint()
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
void MainScreen::HandleEvents(WM_MESSAGE * pMsg)
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
                  case WM_DELETE:
                      asm("nop");
            	  break;
                  case WM_PID_STATE_CHANGED:
                  {

                      volatile WM_PID_STATE_CHANGED_INFO *p = (WM_PID_STATE_CHANGED_INFO*)pMsg->Data.p;
                      GUI_PID_STATE *pState = (GUI_PID_STATE*)pMsg->Data.p;
                      volatile uint32_t x = pState->x;
                      volatile uint32_t y = pState->y;
                      volatile bool pressed = pState->Pressed;

                      asm("nop");
                  }
                  case WM_TOUCH:
                  {
                	  volatile WM_PID_STATE_CHANGED_INFO *p = (WM_PID_STATE_CHANGED_INFO*)pMsg->Data.p;
                      GUI_PID_STATE *pState = (GUI_PID_STATE*)pMsg->Data.p;
                      volatile uint32_t x = pState->x;
                      volatile uint32_t y = pState->y;
                      volatile bool pressed = pState->Pressed;
                      if(!pressed)
                      {
                        display.SetNextState(DisplayStates::set_temp);
                      }
                      asm("nop");
                  }
              //break;
              default:
                old_cb(pMsg);
                  WM_DefaultProc(pMsg);
              }
        }else{
        	switch(pMsg->MsgId)
            {
        		// general touches on the screen that are not on widgets
            	case WM_TOUCH:
            	{
                    GUI_PID_STATE *pState = (GUI_PID_STATE*)pMsg->Data.p;
                    volatile uint32_t x = pState->x;
                    volatile uint32_t y = pState->y;
                    volatile bool pressed = pState->Pressed;
            		asm("nop");
            	}
                break;
            }
            WM_DefaultProc(pMsg);
        }
        display_dirty = true;
	}
	asm("nop");
}
void MainScreen::DoLogic(TickType_t ticks_elapsed)
{
	//auto ms_elapsed = pdTICK_TO_MS(ticks_elapsed);
	volatile uint32_t ticks_per_ms = portTICK_PERIOD_MS;
	asm("nop");
}
