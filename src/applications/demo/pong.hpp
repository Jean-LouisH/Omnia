#pragma once

#include <string>
#include <systems/cpp_scripting_system/cpp_script_instance.hpp> 

namespace OmnificDemo
{
    class Pong : public Omnific::CPPScriptInstance
    {
    public:
        /* Custom classes should derive their own type string
		and update the base type name.*/
		Pong()
		{
			this->type = TYPE_STRING;
		}

		static constexpr const char* TYPE_STRING = "Pong";

		/* Custom classes should have their own clone method 
		for the CPPScriptAssembly instance derivation. */
		virtual CPPScriptInstance* instance() override
		{
			return new Pong(*this);
		}

		/* Add all the methods from CPPScript to override. */
		virtual void on_entity_start() override;
		virtual void on_update() override;
		virtual void on_fixed_update() override;
		virtual void on_late_update() override;

		/* custom public methods. */
	private:
		float acceleration = 5.0;
		float maximum_speed = 30.0;
		float ball_maximum_speed = 100.0;
		float deceleration = 1.5;
		std::string press_start_wait_event = "press_start_wait";
		std::string game_is_playing_event = "game_playing";
		std::string game_paused_event = "game_paused";
		std::string game_set_event = "game_set";

		float player_x_direction = 0.0;
		std::shared_ptr<Omnific::PhysicsBody> ball_physics_body;
		std::shared_ptr<Omnific::Transform> ball_transform;
		std::shared_ptr<Omnific::Entity> ball_entity;
		std::shared_ptr<Omnific::Transform> left_post_transform;
		std::shared_ptr<Omnific::Transform> right_post_transform;
		std::shared_ptr<Omnific::Entity> message_board_entity;
		std::shared_ptr<Omnific::Label> message_board_label;

		std::shared_ptr<Omnific::Label> player_1_score_label;
		std::shared_ptr<Omnific::Label> ai_score_label;
		int player_1_score = 0;
		int ai_score = 0;
		bool player_2_ai_mode = true;

		std::shared_ptr<Omnific::Entity> paddle_1_entity;
		std::shared_ptr<Omnific::Entity> paddle_2_entity;

		std::shared_ptr<Omnific::PhysicsBody> paddle_1_physics_body;
		std::shared_ptr<Omnific::PhysicsBody> paddle_2_physics_body;

		void reset_ball();

    };
}