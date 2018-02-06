/*
 * display_state.cpp
 *
 *  Created on: Feb 4, 2018
 *      Author: joe
 */

#include "display_state.h"

DisplayState::DisplayState(DisplayStates previous_state)
{
	this->previous_state = previous_state;
}
DisplayState::~DisplayState()
{
}
void DisplayState::Paint()
{

}
void DisplayState::HandleEvents(WM_MESSAGE * pMsg)
{

}
void DisplayState::DoLogic(TickType_t ticks_elapsed)
{

}
DisplayStates DisplayState::PreviousState()
{
	return previous_state;
}
