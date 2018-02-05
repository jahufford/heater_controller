/*
 * display_state.h
 *
 *  Created on: Feb 4, 2018
 *      Author: joe
 */

#ifndef DISPLAY_STATE_H_
#define DISPLAY_STATE_H_

#include "WM.h"

class DisplayState
{
    public:
		DisplayState();
		virtual ~DisplayState();
		virtual void Paint();
		virtual void HandleEvents(WM_MESSAGE * pMsg);
    private:
};

#endif /* DISPLAY_STATE_H_ */
