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

#include "core/utilities/aliases.hpp"
#include <set>
#include <queue>
#include "core/utilities/constants.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "core/assets/audio.hpp"
#include "core/component.hpp"
#include "core/assets/audio_synthesis.hpp"


namespace Omnia
{
	class OMNIA_ENGINE_API AudioSource : public Component
	{
	public:
		AudioSource()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "AudioSource";

		virtual Registerable* instance() override
		{
			AudioSource* clone = new AudioSource(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		virtual void deserialize(YAML::Node yaml_node);
		void add_audio(std::shared_ptr<Audio> audio);
		void clear_audio();
		void remove_audio(std::string audio_name);
		void play_audio(std::string audio_name);
		void play();
		void pause();
		void stop();
		void reset();
		uint16_t get_repeat_count();
		void jump(float time_point);
		void set_volume(float value);
		void set_panning(float value);
		float get_volume();
		float get_current_playback_time();
		std::vector<std::string> get_audio_names();
		std::shared_ptr<Audio> get_active_audio();
		std::shared_ptr<Audio> get_audio_by_name(std::string audio_name);
	private:
		std::unordered_map<std::string, std::shared_ptr<Audio>> audio_collection;
		lb_Libretti* libretti;
		std::string active_audio_name;

		bool is_playing_synthesized = false;

		float normalize(float value);
	};
}