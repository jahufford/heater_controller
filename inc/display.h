/*
 * display.h
 *
 *  Created on: Jan 1, 2018
 *      Author: joe
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "WM.h"
#include "BUTTON.h"
#include "display_state.h"
#include <memory>

class Display;

extern Display display;

void Init_Display();

class Display
{
public:
	Display();
	bool Init();
	void RunDisplay();
	void Paint();
	void HandleEvents(WM_MESSAGE * pMsg);
	void ShowStatusBar(bool show_status_bar);
	void SetNextState(DisplayStates next_state);
	void ChangeState();
	static void DisplayCallback(WM_MESSAGE * pMsg);
	DisplayState* CurrentState();
private:
	DisplayStates next_state;
	DisplayState current_state;
	//DisplayState* currentDisplayState;
	std::unique_ptr<DisplayState> currentDisplayState;
//	friend void DisplayCallback(WM_MESSAGE * pMsg);

	bool show_status_bar;
};

#endif /* DISPLAY_H_ */
