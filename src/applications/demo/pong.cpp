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

#include "pong.hpp"
#include "foundations/singletons/random_number_generator.hpp"
#include "systems/cpp_scripting_system/cpp_entity_context.hpp"


void OmnificDemo::Pong::on_entity_start()
{
    Omnific::Platform::get_window().change_title("Omnific Pong");
    std::shared_ptr<Omnific::Scene> scene = Omnific::CPPEntityContext::get_scene();
    this->ball_entity = scene->get_entity_by_name("Ball");
    this->ball_physics_body = scene->get_component_by_type<Omnific::PhysicsBody>(this->ball_entity->get_id());
    this->ball_transform = this->ball_entity->get_transform();
    this->left_post_transform = scene->get_entity_by_name("LeftGoalPost")-> get_transform();
    this->right_post_transform = scene->get_entity_by_name("RightGoalPost")->get_transform();
    this->message_board_entity = scene->get_entity_by_name("MessageBoard");
    this->message_board_label = scene->get_component_by_type<Omnific::Label>(this->message_board_entity->get_id());
    this->reset_ball();
    //Omnific::SceneManager::get_instance()->get_component("AudioSource")->play_infinitely();
    Omnific::EventBus::get_instance()->publish_event(Omnific::Event(this->press_start_wait_event), true);
    this->message_board_label->set_text("Press 'Enter' / 'Start'");

    this->paddle_1_entity = scene->get_entity_by_name("Paddle1");
    this->paddle_2_entity = scene->get_entity_by_name("Paddle2");
    this->paddle_1_physics_body = scene->get_component_by_type<Omnific::PhysicsBody>(scene->get_entity_by_name("Paddle1")->get_id());
    this->paddle_2_physics_body = scene->get_component_by_type<Omnific::PhysicsBody>(scene->get_entity_by_name("Paddle2")->get_id());

    this->player_1_score_label = scene->get_component_by_type<Omnific::Label>(scene->get_entity_by_name("ScoreCount1")->get_id());
    this->ai_score_label = scene->get_component_by_type<Omnific::Label>(scene->get_entity_by_name("ScoreCount2")->get_id());
    this->player_1_score = 0;
    this->ai_score = 0;
    this->player_2_ai_mode = true;
}

void OmnificDemo::Pong::on_update()
{
    Omnific::Inputs& inputs = Omnific::Platform::get_inputs();

    if (Omnific::EventBus::has_event(this->press_start_wait_event))
    {
        if (inputs.is_pressed((std::vector<std::string>){"enter", "button_menu"}))
        {
            Omnific::EventBus::get_instance()->remove_continuous_event(this->press_start_wait_event, "");
            Omnific::EventBus::get_instance()->publish_event(Omnific::Event(this->game_is_playing_event), true);
            this->message_board_label->hide();
            this->reset_ball();
        }
    }

    if (this->player_1_score > 10)
    {
        Omnific::Event::Parameters event_parameters;
        event_parameters.strings = {{"winner", "Player 1"}};
        Omnific::EventBus::get_instance()->publish_event(Omnific::Event(this->game_set_event, event_parameters), true);
        this->player_1_score = 0;
    }

    if (this->ai_score > 10)
    {
        Omnific::Event::Parameters event_parameters;
        event_parameters.strings = {{"winner", "AI"}};
        Omnific::EventBus::get_instance()->publish_event(Omnific::Event(this->game_set_event, event_parameters), true);
        this->ai_score = 0;
    }
}

void OmnificDemo::Pong::on_fixed_update()
{
    if (Omnific::EventBus::query_events_with_string_parameter("entity_is_on_collision", "collider_name", "Ball").size() > 0)
    {
        Omnific::CPPEntityContext::get_component_by_type<Omnific::AudioSource>()->play();
    }

    if (Omnific::EventBus::has_event(this->game_is_playing_event))
    {
        Omnific::Inputs& inputs = Omnific::Platform::get_inputs();
        float ball_bounce_increase = 1.5;

        if ((Omnific::EventBus::has_continuous_event("entity_is_colliding", "Ball_and_UpperBoundary") && this->ball_physics_body->linear_velocity.y > 0.0) || 
            (Omnific::EventBus::has_continuous_event("entity_is_colliding", "Ball_and_LowerBoundary") && this->ball_physics_body->linear_velocity.y < 0.0))
        {
            this->ball_physics_body->linear_velocity.y *= -1.0;
        }

        if (Omnific::EventBus::has_continuous_event("entity_is_colliding", "Ball_and_Paddle1") && this->ball_physics_body->linear_velocity.x < 0.0 || 
            Omnific::EventBus::has_continuous_event("entity_is_colliding", "Ball_and_Paddle2") && this->ball_physics_body->linear_velocity.x > 0.0)
        {
            this->ball_physics_body->linear_velocity.x *= -ball_bounce_increase;
            this->ball_physics_body->linear_velocity.y = Omnific::RandomNumberGenerator::get_random_float(-1.0, 1.0) * Omnific::RandomNumberGenerator::get_random_float(3.0, 30.0);
            if (abs(this->ball_physics_body->linear_velocity.x) > this->ball_maximum_speed)
            {
                if (this->ball_physics_body->linear_velocity.x < 0.0)
                {
                    this->ball_physics_body->linear_velocity.x = -this->ball_maximum_speed;
                }
                else if (this->ball_physics_body->linear_velocity.x > 0.0)
                {
                    this->ball_physics_body->linear_velocity.x = this->ball_maximum_speed;
                }
            }
        }

        if (this->ball_transform->translation.x < this->left_post_transform->translation.x || this->ball_transform->translation.x > this->right_post_transform->translation.x)
        {
            float ball_speed = sqrt(this->ball_physics_body->linear_velocity.x * this->ball_physics_body->linear_velocity.x + 
                this->ball_physics_body->linear_velocity.y * this->ball_physics_body->linear_velocity.y);
                
            Omnific::Event::Parameters event_parameters;
            event_parameters.numbers = {{"ball_speed", ball_speed}};

            if (this->ball_transform->translation.x < this->left_post_transform->translation.x)
            {
                Omnific::EventBus::get_instance()->publish_event(Omnific::Event("AI won", event_parameters), false);
            }
            if (this->ball_transform->translation.x > this->right_post_transform->translation.x)
            {
                Omnific::EventBus::get_instance()->publish_event(Omnific::Event("Player won", event_parameters), false);
            }

            this->reset_ball();
        }

        float control_stick_strength = inputs.get_action_axis("left_axis_y");

        if (inputs.is_action_pressed("dpad_up"))
        {
            this->paddle_1_physics_body->accelerate_y(this->acceleration, this->maximum_speed);
        }
        else if (inputs.is_action_pressed("dpad_down"))
        {
            this->paddle_1_physics_body->accelerate_y(-this->acceleration, this->maximum_speed);
        }
        else if (abs(control_stick_strength) > 0.01)
        {
            this->paddle_1_physics_body->accelerate_y(this->acceleration * control_stick_strength, this->maximum_speed);
        }
        else
        {
            this->paddle_1_physics_body->decelerate_y(this->deceleration);
        }

        if (abs(control_stick_strength) > 0.01)
        {
            this->paddle_1_physics_body->accelerate_y(this->acceleration * control_stick_strength, this->maximum_speed);
        }
        else
        {
            this->paddle_1_physics_body->decelerate_y(this->deceleration);
        }

        if (inputs.is_on_release((std::vector<std::string>){"p", "button_menu"}))
        {
            Omnific::Platform::get_logger().write("P2 enabled");
            this->player_2_ai_mode = !this->player_2_ai_mode;
        }

        if (this->player_2_ai_mode)
        {
            float position_angle_with_ball = atan2(this->ball_transform->translation.y - this->paddle_2_entity->get_transform()->translation.y,
                                                    this->ball_transform->translation.x - this->paddle_2_entity->get_transform()->translation.x);

            if (this->ball_physics_body->linear_velocity.x > 0)
            {
                this->paddle_2_physics_body->accelerate_y(this->acceleration * sin(position_angle_with_ball), this->maximum_speed);
            }
            else
            {
                this->paddle_2_physics_body->decelerate_y(this->deceleration);
            }
        }
        else
        {
            if (inputs.is_pressed((std::vector<std::string>){"up", "button_dpad_up"}))
            {
                this->paddle_2_physics_body->accelerate_y(this->acceleration, this->maximum_speed);
            }
            else if (inputs.is_pressed((std::vector<std::string>){"down", "button_dpad_down"}))
            {
                this->paddle_2_physics_body->accelerate_y(-this->acceleration, -this->maximum_speed);
            }
            else
            {
                this->paddle_2_physics_body->decelerate_y(this->deceleration);
            }
        }        
    }
}

void OmnificDemo::Pong::on_late_update()
{
    for (Omnific::Event event : Omnific::EventBus::query_events(this->game_set_event))
    {
        std::string winner = event.get_parameters().strings["winner"];
        this->message_board_label->set_text(winner + " is the winner! Press 'Enter' / 'Start' to play again.");
        this->message_board_label->show();
        Omnific::EventBus::get_instance()->publish_event(Omnific::Event(this->press_start_wait_event), true);
        Omnific::EventBus::get_instance()->remove_continuous_event(this->game_is_playing_event, "");
        Omnific::EventBus::get_instance()->remove_continuous_event(this->game_set_event, "");
    }

    if (Omnific::EventBus::has_event("Player won"))
    {
        this->player_1_score += 1;
    }

    if (Omnific::EventBus::has_event("AI won"))
    {
        this->ai_score += 1;
    }

    if (Omnific::Platform::get_inputs().is_on_release((std::vector<std::string>){"p", "button_menu"}))
    {
        this->player_2_ai_mode = !this->player_2_ai_mode;
    }

    std::string player_name = "AI";

    if (!this->player_2_ai_mode)
    {
        player_name = "Player 2";
    }

    this->player_1_score_label->set_text("Player 1: " + std::to_string(this->player_1_score));
    this->ai_score_label->set_text(player_name + ": " + std::to_string(this->ai_score));

}

void OmnificDemo::Pong::reset_ball()
{
    this->player_x_direction = Omnific::RandomNumberGenerator::get_random_boolean();
    this->ball_physics_body->linear_velocity.x = Omnific::RandomNumberGenerator::get_random_float(12.0, 14.0) * (this->player_x_direction ? 1.0 : -1.0);
    this->ball_physics_body->linear_velocity.y = Omnific::RandomNumberGenerator::get_random_float(5.0, 8.0);
    this->ball_transform->translation.x = 0.0;
    this->ball_transform->translation.y = 1.0;
    this->ball_transform->translation.z = 0.0;
}