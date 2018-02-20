/*
 * settings_dstate.h
 *
 *  Created on: Feb 18, 2018
 *      Author: joe
 */

#ifndef SETTINGS_DSTATE_H_
#define SETTINGS_DSTATE_H_

#include "stm32f4xx.h"
#include "display_state.h"
#include "BUTTON.h"

class SettingsDState : public DisplayState
{
public:
	SettingsDState(DisplayStates previous_state=DisplayStates::null);
	virtual ~SettingsDState();
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

#endif /* SETTINGS_DSTATE_H_ */
