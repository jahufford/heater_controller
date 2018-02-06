/*
 * main_screen.h
 *
 *  Created on: Feb 4, 2018
 *      Author: joe
 */

#ifndef MAIN_SCREEN_H_
#define MAIN_SCREEN_H_

#include "stm32f4xx.h"
#include "display_state.h"
#include "BUTTON.h"

class MainScreen : public DisplayState
{
public:
	MainScreen(DisplayStates previous_state=DisplayStates::null);
	virtual ~MainScreen();
	virtual void Paint();
	virtual void HandleEvents(WM_MESSAGE * pMsg);
	virtual void DoLogic(TickType_t ticks_elapsed);
private:
	bool display_dirty;
	BUTTON_Handle button;
	WM_CALLBACK *old_cb;
	uint32_t current_temp;
   	char sent_char;
  	char recv_char;
};

#endif /* MAIN_SCREEN_H_ */
