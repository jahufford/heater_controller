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
	MainScreen();
	virtual ~MainScreen();
	virtual void Paint();
	virtual void HandleEvents(WM_MESSAGE * pMsg);
private:
	BUTTON_Handle button;
	WM_CALLBACK *old_cb;
};

#endif /* MAIN_SCREEN_H_ */
