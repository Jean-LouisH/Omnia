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

#pragma once

#include "foundations/aliases.hpp"
#include "foundations/constants.hpp"
#include "scene/components/component.hpp"


namespace Omnific
{
	class OMNIFIC_ENGINE_API AudioListener : public Component
	{
	public:
		AudioListener()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "AudioListener";

		virtual Registerable* instance() override
		{
			AudioListener* clone = new AudioListener(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}
		std::vector<uint8_t> spectrum_data;

		virtual void deserialize(YAML::Node yaml_node);
		void set_volume(float value);
		float get_volume();
		void mute();
		void unmute();
	private:
		float volume = 1.0;
		float previous_volume = 1.0;
	};
}