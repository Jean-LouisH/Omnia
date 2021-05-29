// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", InputCode::WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "input_api.hpp"
#include <os/os.hpp>

Lilliputian::InputAPI::InputAPI()
{
	this->analogueStickSensitivity = 4096;

	this->controllerButtonsByString.emplace("dpad_left", SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	this->controllerButtonsByString.emplace("dpad_right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	this->controllerButtonsByString.emplace("dpad_up", SDL_CONTROLLER_BUTTON_DPAD_UP);
	this->controllerButtonsByString.emplace("dpad_down", SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	this->controllerButtonsByString.emplace("button_extra", SDL_CONTROLLER_BUTTON_BACK);
	this->controllerButtonsByString.emplace("button_menu", SDL_CONTROLLER_BUTTON_START);
	this->controllerButtonsByString.emplace("button_x", SDL_CONTROLLER_BUTTON_X);
	this->controllerButtonsByString.emplace("button_y", SDL_CONTROLLER_BUTTON_Y);
	this->controllerButtonsByString.emplace("button_a", SDL_CONTROLLER_BUTTON_A);
	this->controllerButtonsByString.emplace("button_b", SDL_CONTROLLER_BUTTON_B);
	this->controllerButtonsByString.emplace("left_shoulder_button", SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	this->controllerButtonsByString.emplace("left_stick_button", SDL_CONTROLLER_BUTTON_LEFTSTICK);
	this->controllerButtonsByString.emplace("right_shoulder_button", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	this->controllerButtonsByString.emplace("right_stick_button", SDL_CONTROLLER_BUTTON_RIGHTSTICK);

	this->controllerAxisEventsByString.emplace("left_axis_x", SDL_CONTROLLER_AXIS_LEFTX);
	this->controllerAxisEventsByString.emplace("left_axis_y", SDL_CONTROLLER_AXIS_LEFTY);
	this->controllerAxisEventsByString.emplace("right_axis_x", SDL_CONTROLLER_AXIS_RIGHTX);
	this->controllerAxisEventsByString.emplace("right_axis_y", SDL_CONTROLLER_AXIS_RIGHTY);
	this->controllerAxisEventsByString.emplace("left_trigger", SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	this->controllerAxisEventsByString.emplace("right_trigger", SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

	this->keyboardEventsByString.emplace("escape", SDLK_ESCAPE);
	this->keyboardEventsByString.emplace("f1", SDLK_F1);
	this->keyboardEventsByString.emplace("f2", SDLK_F2);
	this->keyboardEventsByString.emplace("f3", SDLK_F3);
	this->keyboardEventsByString.emplace("f4", SDLK_F4);
	this->keyboardEventsByString.emplace("f5", SDLK_F5);
	this->keyboardEventsByString.emplace("f6", SDLK_F6);
	this->keyboardEventsByString.emplace("f7", SDLK_F7);
	this->keyboardEventsByString.emplace("f8", SDLK_F8);
	this->keyboardEventsByString.emplace("f9", SDLK_F9);
	this->keyboardEventsByString.emplace("f10", SDLK_F10);
	this->keyboardEventsByString.emplace("f11", SDLK_F11);
	this->keyboardEventsByString.emplace("f12", SDLK_F12);
	this->keyboardEventsByString.emplace("printscreen", SDLK_PRINTSCREEN);
	this->keyboardEventsByString.emplace("pause", SDLK_PAUSE);
	this->keyboardEventsByString.emplace("insert", SDLK_INSERT);
	this->keyboardEventsByString.emplace("delete", SDLK_DELETE);
	this->keyboardEventsByString.emplace("home", SDLK_HOME);
	this->keyboardEventsByString.emplace("page_up", SDLK_PAGEUP);
	this->keyboardEventsByString.emplace("page_down", SDLK_PAGEDOWN);
	this->keyboardEventsByString.emplace("end", SDLK_END);
	this->keyboardEventsByString.emplace("backquote", SDLK_BACKQUOTE);
	this->keyboardEventsByString.emplace("1", SDLK_1);
	this->keyboardEventsByString.emplace("2", SDLK_2);
	this->keyboardEventsByString.emplace("3", SDLK_3);
	this->keyboardEventsByString.emplace("4", SDLK_4);
	this->keyboardEventsByString.emplace("5", SDLK_5);
	this->keyboardEventsByString.emplace("6", SDLK_6);
	this->keyboardEventsByString.emplace("7", SDLK_7);
	this->keyboardEventsByString.emplace("8", SDLK_8);
	this->keyboardEventsByString.emplace("9", SDLK_9);
	this->keyboardEventsByString.emplace("0", SDLK_0);
	this->keyboardEventsByString.emplace("-", SDLK_MINUS);
	this->keyboardEventsByString.emplace("+", SDLK_PLUS);
	this->keyboardEventsByString.emplace("backspace", SDLK_BACKSPACE);
	this->keyboardEventsByString.emplace("num_lock", SDLK_NUMLOCKCLEAR);
	this->keyboardEventsByString.emplace("forward_slash", SDLK_SLASH);
	this->keyboardEventsByString.emplace("asterisk", SDLK_ASTERISK);
	this->keyboardEventsByString.emplace("keypad_minus", SDLK_KP_MINUS);
	this->keyboardEventsByString.emplace("keypad_plus", SDLK_KP_PLUS);
	this->keyboardEventsByString.emplace("keypad_enter", SDLK_KP_ENTER);
	this->keyboardEventsByString.emplace("keypad_1", SDLK_KP_1);
	this->keyboardEventsByString.emplace("keypad_2", SDLK_KP_2);
	this->keyboardEventsByString.emplace("keypad_3", SDLK_KP_3);
	this->keyboardEventsByString.emplace("keypad_4", SDLK_KP_4);
	this->keyboardEventsByString.emplace("keypad_5", SDLK_KP_5);
	this->keyboardEventsByString.emplace("keypad_6", SDLK_KP_6);
	this->keyboardEventsByString.emplace("keypad_7", SDLK_KP_7);
	this->keyboardEventsByString.emplace("keypad_8", SDLK_KP_8);
	this->keyboardEventsByString.emplace("keypad_9", SDLK_KP_9);
	this->keyboardEventsByString.emplace("keypad_0", SDLK_KP_0);
	this->keyboardEventsByString.emplace("keypad_period", SDLK_KP_PERIOD);
	this->keyboardEventsByString.emplace("tab", SDLK_TAB);
	this->keyboardEventsByString.emplace("q", SDLK_q);
	this->keyboardEventsByString.emplace("w", SDLK_w);
	this->keyboardEventsByString.emplace("e", SDLK_e);
	this->keyboardEventsByString.emplace("r", SDLK_r);
	this->keyboardEventsByString.emplace("t", SDLK_t);
	this->keyboardEventsByString.emplace("y", SDLK_y);
	this->keyboardEventsByString.emplace("u", SDLK_u);
	this->keyboardEventsByString.emplace("i", SDLK_i);
	this->keyboardEventsByString.emplace("o", SDLK_o);
	this->keyboardEventsByString.emplace("p", SDLK_p);
	this->keyboardEventsByString.emplace("a", SDLK_a);
	this->keyboardEventsByString.emplace("s", SDLK_s);
	this->keyboardEventsByString.emplace("d", SDLK_d);
	this->keyboardEventsByString.emplace("f", SDLK_f);
	this->keyboardEventsByString.emplace("g", SDLK_g);
	this->keyboardEventsByString.emplace("h", SDLK_h);
	this->keyboardEventsByString.emplace("j", SDLK_j);
	this->keyboardEventsByString.emplace("k", SDLK_k);
	this->keyboardEventsByString.emplace("l", SDLK_l);
	this->keyboardEventsByString.emplace("z", SDLK_z);
	this->keyboardEventsByString.emplace("x", SDLK_x);
	this->keyboardEventsByString.emplace("c", SDLK_c);
	this->keyboardEventsByString.emplace("v", SDLK_v);
	this->keyboardEventsByString.emplace("b", SDLK_b);
	this->keyboardEventsByString.emplace("n", SDLK_n);
	this->keyboardEventsByString.emplace("m", SDLK_m);
	this->keyboardEventsByString.emplace("left_square_bracket", SDLK_LEFTBRACKET);
	this->keyboardEventsByString.emplace("right_square_bracket", SDLK_RIGHTBRACKET);
	this->keyboardEventsByString.emplace("semicolon", SDLK_SEMICOLON);
	this->keyboardEventsByString.emplace("quote", SDLK_QUOTE);
	this->keyboardEventsByString.emplace("backslash", SDLK_BACKSLASH);
	this->keyboardEventsByString.emplace("enter", SDLK_RETURN);
	this->keyboardEventsByString.emplace("comma", SDLK_COMMA);
	this->keyboardEventsByString.emplace("period", SDLK_PERIOD);
	this->keyboardEventsByString.emplace("left_shift", SDLK_LSHIFT);
	this->keyboardEventsByString.emplace("right_shift", SDLK_RSHIFT);
	this->keyboardEventsByString.emplace("left_control", SDLK_LCTRL);
	this->keyboardEventsByString.emplace("right_control", SDLK_RCTRL);
	this->keyboardEventsByString.emplace("function", SDLK_f);
	this->keyboardEventsByString.emplace("os", SDLK_LGUI);
	this->keyboardEventsByString.emplace("left_alt", SDLK_LALT);
	this->keyboardEventsByString.emplace("space", SDLK_SPACE);
	this->keyboardEventsByString.emplace("right_alt", SDLK_RALT);
	this->keyboardEventsByString.emplace("left", SDLK_LEFT);
	this->keyboardEventsByString.emplace("up", SDLK_UP);
	this->keyboardEventsByString.emplace("right", SDLK_RIGHT);
	this->keyboardEventsByString.emplace("down", SDLK_DOWN);

/*
	*/

	/*Default action input map*/
	//this->insertActionInput("dpad left", InputCode::GAME_CONTROLLER_BUTTON_DPAD_LEFT);
	//this->insertActionInput("dpad left", InputCode::KEY_F);
	//this->insertActionInput("dpad right", InputCode::GAME_CONTROLLER_BUTTON_DPAD_RIGHT);
	//this->insertActionInput("dpad right", InputCode::KEY_H);
	//this->insertActionInput("dpad up", InputCode::GAME_CONTROLLER_BUTTON_DPAD_UP);
	//this->insertActionInput("dpad up", InputCode::KEY_T);
	//this->insertActionInput("dpad down", InputCode::GAME_CONTROLLER_BUTTON_DPAD_DOWN);
	//this->insertActionInput("dpad down", InputCode::KEY_G);
	//this->insertActionInput("extra", InputCode::GAME_CONTROLLER_BUTTON_EXTRA);
	//this->insertActionInput("extra", InputCode::KEY_E);
	//this->insertActionInput("menu", InputCode::GAME_CONTROLLER_BUTTON_MENU);
	//this->insertActionInput("menu", InputCode::KEY_U);
	//this->insertActionInput("x", InputCode::GAME_CONTROLLER_BUTTON_X);
	//this->insertActionInput("x", InputCode::KEY_J);
	//this->insertActionInput("y", InputCode::GAME_CONTROLLER_BUTTON_Y);
	//this->insertActionInput("y", InputCode::KEY_I);
	//this->insertActionInput("a", InputCode::GAME_CONTROLLER_BUTTON_A);
	//this->insertActionInput("a", InputCode::KEY_K);
	//this->insertActionInput("b", InputCode::GAME_CONTROLLER_BUTTON_B);
	//this->insertActionInput("b", InputCode::KEY_L);
	//this->insertActionInput("left shoulder", InputCode::GAME_CONTROLLER_BUTTON_LEFT_SHOULDER);
	//this->insertActionInput("left shoulder", InputCode::KEY_Q);
	//this->insertActionInput("left stick", InputCode::GAME_CONTROLLER_BUTTON_LEFT_STICK_BUTTON);
	//this->insertActionInput("left stick", InputCode::KEY_C);
	//this->insertActionInput("right shoulder", InputCode::GAME_CONTROLLER_BUTTON_RIGHT_SHOULDER);
	//this->insertActionInput("right shoulder", InputCode::KEY_O);
	//this->insertActionInput("right stick", InputCode::GAME_CONTROLLER_BUTTON_RIGHT_STICK_BUTTON);
	//this->insertActionInput("right stick", InputCode::KEY_N);
	//this->insertActionInput("left axis left", InputCode::GAME_CONTROLLER_LEFT_AXIS_LEFT);
	//this->insertActionInput("left axis left", InputCode::KEY_A);
	//this->insertActionInput("left axis right", InputCode::GAME_CONTROLLER_LEFT_AXIS_RIGHT);
	//this->insertActionInput("left axis right", InputCode::KEY_D);
	//this->insertActionInput("left axis up", InputCode::GAME_CONTROLLER_LEFT_AXIS_UP);
	//this->insertActionInput("left axis up", InputCode::KEY_W);
	//this->insertActionInput("left axis down", InputCode::GAME_CONTROLLER_LEFT_AXIS_DOWN);
	//this->insertActionInput("left axis down", InputCode::KEY_S);
	//this->insertActionInput("right axis left", InputCode::GAME_CONTROLLER_RIGHT_AXIS_LEFT);
	//this->insertActionInput("right axis left", InputCode::KEY_SEMICOLON);
	//this->insertActionInput("right axis right", InputCode::GAME_CONTROLLER_RIGHT_AXIS_RIGHT);
	//this->insertActionInput("right axis right", InputCode::KEY_BACKSLASH);
	//this->insertActionInput("right axis up", InputCode::GAME_CONTROLLER_RIGHT_AXIS_UP);
	//this->insertActionInput("right axis up", InputCode::KEY_QUOTE);
	//this->insertActionInput("right axis down", InputCode::GAME_CONTROLLER_RIGHT_AXIS_DOWN);
	//this->insertActionInput("right axis down", InputCode::KEY_OPEN_SQUARE_BRACKET);
	//this->insertActionInput("left trigger", InputCode::GAME_CONTROLLER_TRIGGER_LEFT_AXIS);
	//this->insertActionInput("left trigger", InputCode::KEY_LEFT_SHIFT);
	//this->insertActionInput("right trigger", InputCode::GAME_CONTROLLER_TRIGGER_RIGHT_AXIS);
	//this->insertActionInput("right trigger", InputCode::KEY_RIGHT_SHIFT);
}

void Lilliputian::InputAPI::initialize(HumanInterfaceDevices* hid)
{
	this->hid = hid;
}

//void Lilliputian::InputAPI::insertActionInput(String actionName, InputCode inputCode)
//{
//	String actionString = actionName;
//
//	if (this->actionMappedInputs.count(actionName))
//	{
//		this->actionMappedInputs.at(actionName).push_back(inputCode);
//	}
//	else
//	{
//		Vector<InputCode> inputCodes;
//		inputCodes.push_back(inputCode);
//		this->actionMappedInputs.emplace(actionString, inputCodes);
//	}
//}
//
//void Lilliputian::InputAPI::replaceActionInput(String actionName, InputCode inputCode)
//{
//	String actionString = actionName;
//
//	if (this->actionMappedInputs.count(actionString))
//	{
//		this->actionMappedInputs.at(actionString).clear();
//		this->actionMappedInputs.at(actionString).push_back(inputCode);
//	}
//	else
//	{
//		Vector<InputCode> inputCodes;
//		inputCodes.push_back(inputCode);
//		this->actionMappedInputs.emplace(actionString, inputCodes);
//	}
//
//}
//
//void Lilliputian::InputAPI::removeActionInput(String actionName, InputCode inputCode)
//{
//
//}
//
//void Lilliputian::InputAPI::removeAllActionInputs(String actionName)
//{
//	String actionString = actionName;
//	if (this->actionMappedInputs.count(actionString))
//		this->actionMappedInputs.at(actionString).clear();
//}
//
//Lilliputian::Map<Lilliputian::String, Lilliputian::Vector<Lilliputian::InputCode>> Lilliputian::InputAPI::getActionInputs()
//{
//	return this->actionMappedInputs;
//}

bool Lilliputian::InputAPI::isOnPress(String keyCode)
{
	Map<SDL_Keycode, SDL_KeyboardEvent> keyboardEvents = this->hid->getKeyboardEvents();
	Map<Lilliputian::HumanInterfaceDevices::ControllerButtonCode, SDL_ControllerButtonEvent> controllerButtonEvents =
		this->hid->getControllerButtonEvents();

	if (this->keyboardEventsByString.count(keyCode))
	{
		SDL_Keycode sdlKeyCode = this->keyboardEventsByString.at(keyCode);
		if (keyboardEvents.count(sdlKeyCode))
			if (keyboardEvents.at(sdlKeyCode).type == SDL_KEYDOWN)
				return true;
	}

	if (this->controllerButtonsByString.count(keyCode))
	{
		Lilliputian::HumanInterfaceDevices::ControllerButtonCode controllerButtonCode = this->controllerButtonsByString.at(keyCode);
		if (controllerButtonEvents.count(controllerButtonCode))
			if (controllerButtonEvents.at(controllerButtonCode).type == SDL_CONTROLLERBUTTONDOWN)
				return true;
	}

	return false;
}

bool Lilliputian::InputAPI::isOnDoublePress(String keyCode, unsigned int timeInterval_ms)
{
	return false;
}

bool Lilliputian::InputAPI::isOnRelease(String keyCode)
{
	Map<SDL_Keycode, SDL_KeyboardEvent> keyboardEvents = this->hid->getKeyboardEvents();
	Map<Lilliputian::HumanInterfaceDevices::ControllerButtonCode, SDL_ControllerButtonEvent> controllerButtonEvents =
		this->hid->getControllerButtonEvents();

	if (this->keyboardEventsByString.count(keyCode))
	{
		SDL_Keycode sdlKeyCode = this->keyboardEventsByString.at(keyCode);
		if (keyboardEvents.count(sdlKeyCode))
			if (keyboardEvents.at(sdlKeyCode).type == SDL_KEYUP)
				return true;
	}

	if (this->controllerButtonsByString.count(keyCode))
	{
		Lilliputian::HumanInterfaceDevices::ControllerButtonCode controllerButtonCode = this->controllerButtonsByString.at(keyCode);
		if (controllerButtonEvents.count(controllerButtonCode))
			if (controllerButtonEvents.at(controllerButtonCode).type == SDL_CONTROLLERBUTTONUP)
				return true;
	}

	return false;
}

bool Lilliputian::InputAPI::isOnHold(String keyCode)
{
	return false;
}

//bool Lilliputian::InputAPI::isActionOnPress(String actionName)
//{
//	return false;
//}
//
//bool Lilliputian::InputAPI::isActionOnDoublePress(String actionName, unsigned int timeInterval_ms)
//{
//	return false;
//}
//
//bool Lilliputian::InputAPI::isActionOnRelease(String actionName)
//{
//	return false;
//}
//
//bool Lilliputian::InputAPI::isActionOnHold(String actionName, unsigned int timeInterval_ms)
//{
//	return false;
//}

float Lilliputian::InputAPI::getAxisStrength(String axisActionName)
{
	return 0.0;
}