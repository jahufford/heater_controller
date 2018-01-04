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

class Display;

extern Display display;

class Display
{
public:
	Display();
	void RunDisplay();
	void Paint();
	void HandleEvents(WM_MESSAGE * pMsg);
	void ShowStatusBar(bool show_status_bar);
private:
	friend void DisplayCallback(WM_MESSAGE * pMsg);
	BUTTON_Handle button;
	bool show_status_bar;
};

#endif /* DISPLAY_H_ */
