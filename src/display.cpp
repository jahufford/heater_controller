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
#include "main_screen_dstate.h"
#include "set_temp_dstate.h"
#include "settings_dstate.h"

#include "wireless_uart.h"
//#include <memory>



Display display;
//Display* display;
//uint8_t *display_obj_mem;
//void Init_Display()
//{
//	display_obj_mem = static_cast<uint8_t*>(pvPortMalloc(sizeof(Display)));
//	display = new (display_obj_mem) Display;
//	asm("nop");
//}
void Display::DisplayCallback(WM_MESSAGE * pMsg)
{
	display.CurrentState()->HandleEvents(pMsg);
}
DisplayState* Display::CurrentState()
{
	return currentDisplayState.get();
}
Display::Display()
{
	asm("nop");
}
bool Display::Init()
{
	// use an Init function because the constructor is called before the tickers are started
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
    currentDisplayState = std::make_unique<MainScreen>();
    //currentDisplayState.reset(new DisplayState());

  //  currentDisplayState = make_u new MainScreen;
//    GUI_DispString("Hello world!");
//    GUI_DispDecAt( 27, 20,20,4);
//
//
////    volatile int x = GUI_IsInitialized();
//    GUI_SetBkColor(GUI_DARKRED);
//    GUI_Clear();
//    asm("nop");
//    //GUI_FillRect(20,20,280,150);
//    //GUI_SetBkColor(GUI_DARKRED);
//    //GUI_SetColor(LCD_MakeColor(GUI_BLUE));
//    GUI_SetColor(GUI_MAKE_COLOR(GUI_RED));
//    uint32_t colors[25] = {GUI_BLUE,GUI_GREEN,GUI_RED, GUI_CYAN,GUI_MAGENTA,GUI_YELLOW,
//    					   GUI_LIGHTBLUE,GUI_LIGHTGREEN,GUI_LIGHTRED,GUI_LIGHTCYAN, GUI_LIGHTMAGENTA,
//						   GUI_LIGHTYELLOW,GUI_DARKBLUE,GUI_DARKGREEN,GUI_DARKRED,GUI_DARKCYAN,
//						   GUI_DARKMAGENTA,GUI_DARKYELLOW,GUI_WHITE,GUI_LIGHTGRAY,GUI_GRAY,
//						   GUI_DARKGRAY,GUI_BLACK,GUI_BROWN,GUI_ORANGE};
//    GUI_SetColor(GUI_WHITE);
//    GUI_DrawRect(0,0,319,239);
////    uint8_t color_index = 0;
////    for(int i=10;i<310/2;i+=10){
////    	GUI_SetColor(colors[color_index]);
////    	color_index++;
////    	color_index%=25;
////    	GUI_DrawRect(i,i,319-i,239-i);
////    }
////    GUI_SetColor(GUI_RED);
////    GUI_FillRect(20,20,80,40);
////
////    GUI_SetColor(GUI_BLUE);
////    GUI_SetBkColor(GUI_GREEN);
////    GUI_DispCharAt('A',3,200);
////    GUI_DispStringAt("Hi", (LCD_GetXSize()-100)/2, (LCD_GetYSize()-20)/2);
////    GUI_SetBkColor(GUI_CYAN);
////    GUI_DispStringAt("Hello World!", (LCD_GetXSize()-100)/2, 200);
////    GUI_SetColor(GUI_YELLOW);
////    GUI_SetFont(GUI_FONT_COMIC24B_1);
////    GUI_DispStringAt("Hello World!", 95,150);
//    GUI_SetColor(GUI_YELLOW);
//    GUI_SetBkColor(GUI_BLACK);
////    WM_Exec();
////    GUI_Exec();
//



}
void Display::RunDisplay()
{
	// one possibility would be put the classic game loop here
	// paint()
	// handleevents()
	// dologic()
	// changestate()
	// in order to do that, since handle_events is event driven with a callback,
	// just have the callback repackage and push the events into a queue from which
	// this version of handleevents() pulls.
	// or maybe not

	//Paint();
	uint32_t ticks = HAL_GetTick();
	while(1)
	{
		currentDisplayState->HandleEvents(NULL);
		currentDisplayState->DoLogic(HAL_GetTick() - ticks);
		currentDisplayState->Paint();
		ChangeState();
		ticks = HAL_GetTick();
	}
}
void Display::Paint()
{
	// paint the status bar
	if(show_status_bar)
	{

	}
	currentDisplayState->Paint();

}

void Display::HandleEvents(WM_MESSAGE * pMsg)
{

}

void Display::ShowStatusBar(bool show_status_bar)
{
	this->show_status_bar = show_status_bar;
}
void Display::SetNextState(DisplayStates new_state)
{
	if(next_state != DisplayStates::exit)
	{
		next_state = new_state;
	}
}
void Display::ChangeState()
{
	if(next_state != DisplayStates::null)
	{
		if(next_state != DisplayStates::exit)
		{
			switch(next_state)
			{
				// using smart pointers so destructors get called
				case DisplayStates::main_screen:
					currentDisplayState = std::make_unique<MainScreen>();
		break;
				case DisplayStates::set_temp:
//					delete currentDisplayState;
					currentDisplayState = std::make_unique<SetTempDState>();
				break;
				case DisplayStates::settings:
					currentDisplayState = std::make_unique<SettingsDState>();
				break;
			}
			next_state = DisplayStates::null;
		}
	}
}
