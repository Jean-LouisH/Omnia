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

#include "audio_player.hpp"
#include "foundations/singletons/platform/platform.hpp"
#include "foundations/singletons/random_number_generator.hpp"
#include "systems/cpp_scripting_system/cpp_entity_context.hpp"


void OmnificDemo::AudioPlayer::on_entity_start()
{
    Omnific::Platform::get_window().change_title("Omnific Audio Player");
    std::shared_ptr<Omnific::Scene> scene = Omnific::CPPEntityContext::get_scene();
    this->audio_source = Omnific::CPPEntityContext::get_component_by_type<Omnific::AudioSource>();
    this->audio_source->is_capturing_waveform = true;
    this->audio_source->waveform_capture_sample_count = static_cast<int>(Omnific::Platform::get_window().get_window_size().x / 8);
    this->playback_time_label = scene->get_component_by_type_from_entity_by_name<Omnific::Label>("Playback Time");
    this->playback_length_label = scene->get_component_by_type_from_entity_by_name<Omnific::Label>("Playback Length");
    this->oscilloscope_plot_canvas = scene->get_component_by_type_from_entity_by_name<Omnific::PlotCanvas>("Oscilloscope");
    this->start_message_label = scene->get_component_by_type_from_entity_by_name<Omnific::Label>("Start Message");
    this->play_pause_button = scene->get_component_by_type_from_entity_by_name<Omnific::Button>("Play/Pause Button");
    this->previous_button = scene->get_component_by_type_from_entity_by_name<Omnific::Button>("Previous Button");
    this->next_button = scene->get_component_by_type_from_entity_by_name<Omnific::Button>("Next Button");
    std::vector<std::string> args = Omnific::Platform::get_command_line_arguments();
    if (args.size() > 1)
    {
        for (size_t i = 1; i < args.size(); ++i)
        {
            if (Omnific::Platform::get_file_access().exists(args[i]))
            {
                this->playlist.push_back(args[i]);
                this->audio_source->add_audio(std::make_shared<Omnific::Audio>(args[i]));
            }
        }

        if (this->playlist.size() > 0)
        {
            this->audio_source->play_audio(this->playlist[0]);
        }
    }
}

void OmnificDemo::AudioPlayer::on_update()
{
    std::shared_ptr<Omnific::Audio> active_audio = this->audio_source->get_active_audio();
    if (active_audio != nullptr)
    {
        float current_playback_time = this->audio_source->get_current_playback_time();
        float playback_length = this->audio_source->get_playback_length();
        this->playback_time_label->set_text(std::to_string(static_cast<int>(current_playback_time / 60)).append(":").append(std::to_string(static_cast<int>(current_playback_time) % 60)));
        this->playback_length_label->set_text(std::to_string(static_cast<int>(playback_length / 60)).append(":").append(std::to_string(static_cast<int>(playback_length) % 60)));
        this->oscilloscope_plot_canvas->set_plot_points(this->audio_source->get_current_waveform(), std::shared_ptr<Omnific::Colour>(new Omnific::Colour((uint8_t)255, 255, 255)));
        this->start_message_label->hide();
    }

    if (this->play_pause_button->is_clicked && Omnific::Platform::get_inputs().is_left_mouse_button_on_press())
    {
        if (this->audio_source->is_playing())
        {
            this->audio_source->pause();
        }
        else
        {
            this->audio_source->resume();
        }
    }
}

void OmnificDemo::AudioPlayer::on_fixed_update()
{

}

void OmnificDemo::AudioPlayer::on_late_update()
{
    std::vector<Omnific::Event> file_drop_events = Omnific::EventBus::query_events("file_dropped_on_window");
    if (file_drop_events.size() > 0)
    {
        std::string audio_filepath = file_drop_events[0].get_parameters().strings["drop_file_path"];
        if (Omnific::Platform::get_file_access().exists(audio_filepath))
        {
            this->start_message_label->set_text("Loading: " + audio_filepath);
            this->audio_source->add_audio(std::make_shared<Omnific::Audio>(audio_filepath));
            this->playlist.push_back(audio_filepath);
            if (!this->audio_source->is_playing())
            {
                this->audio_source->play_audio(audio_filepath);
            }
        }
    }
}