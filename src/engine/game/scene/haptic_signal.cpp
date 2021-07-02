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
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "haptic_signal.hpp"

Lilliputian::HapticSignal::HapticSignal(ControllerPlayerID playerID, float strength_pct, uint16_t duration_ms)
{
	uint16_t durationLimit = 2000;

	this->playerID = playerID;
	
	if (strength_pct < 0.0)
		this->strength_pct = 0.0;
	else if (strength_pct > 1.0)
		this->strength_pct = 1.0;
	else
		this->strength_pct = strength_pct;

	if (duration_ms > durationLimit)
		this->duration_ms = durationLimit;
	else
		this->duration_ms = duration_ms;
}

Lilliputian::ControllerPlayerID Lilliputian::HapticSignal::getPlayerID()
{
	return this->playerID;
}

float Lilliputian::HapticSignal::getStrength_pct()
{
	return this->strength_pct;
}

uint16_t Lilliputian::HapticSignal::getDuration_ms()
{
	return this->duration_ms;
}