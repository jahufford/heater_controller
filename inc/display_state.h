/*
 * display_state.h
 *
 *  Created on: Feb 4, 2018
 *      Author: joe
 */

#ifndef DISPLAY_STATE_H_
#define DISPLAY_STATE_H_

#include "WM.h"

enum class DisplayStates { null, main_screen, set_temp };

class DisplayState
{
    public:
		DisplayState(DisplayStates previous_state=DisplayStates::null);
		virtual ~DisplayState();
		virtual void Paint();
		virtual void HandleEvents(WM_MESSAGE * pMsg);
    private:
		DisplayStates previous_state;
};

#endif /* DISPLAY_STATE_H_ */
