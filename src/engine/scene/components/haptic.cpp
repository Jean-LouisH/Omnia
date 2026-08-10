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

#include "haptic.hpp"
#include <foundations/singletons/event_bus.hpp>

void Omnific::Haptic::deserialize(YAML::Node yaml_node)
{
	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "player_id")
		{
			this->player_id = it3->second.as<int>();
		}
		else if (it3->first.as<std::string>() == "duration")
		{
			this->duration = it3->second.as<int>();
		}
		else if (it3->first.as<std::string>() == "strength")
		{
			this->strength = it3->second.as<float>();
		}
	}
}

void Omnific::Haptic::rumble(uint16_t duration, float strength, PlayerID player_id)
{
	this->duration = duration;
	this->strength = strength;
	this->player_id = player_id;

	EventBus::publish_event(OMNIFIC_EVENT_HAPTIC_SIGNAL, {}, {
		{"duration", (double)duration},
		{"strength", (double)strength},
		{"player_id", (double)player_id}
	});
}