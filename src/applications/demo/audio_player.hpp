#pragma once

#include <string>
#include <systems/cpp_scripting_system/cpp_script_instance.hpp> 

namespace OmnificDemo
{
    class AudioPlayer : public Omnific::CPPScriptInstance
    {
    public:
        /* Custom classes should derive their own type string
		and update the base type name.*/
		AudioPlayer()
		{
			this->type = TYPE_STRING;
		}

		static constexpr const char* TYPE_STRING = "AudioPlayer";

		/* Custom classes should have their own clone method 
		for the CPPScriptAssembly instance derivation. */
		virtual CPPScriptInstance* instance() override
		{
			return new AudioPlayer(*this);
		}

		/* Add all the methods from CPPScript to override. */
		virtual void on_entity_start() override;
		virtual void on_update() override;
		virtual void on_fixed_update() override;
		virtual void on_late_update() override;

		/* custom public methods. */
	private:
		std::string input_string;
		std::vector<std::string> playlist;
		std::shared_ptr<Omnific::AudioSource> audio_source;
		std::shared_ptr<Omnific::Label> playback_time_label;
		std::shared_ptr<Omnific::Label> playback_length_label;
		std::shared_ptr<Omnific::PlotCanvas> oscilloscope_plot_canvas;
		std::shared_ptr<Omnific::Label> start_message_label;
		std::shared_ptr<Omnific::Button> play_pause_button;
		std::shared_ptr<Omnific::Button> previous_button;
		std::shared_ptr<Omnific::Button> next_button;
    };
}