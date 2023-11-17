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
#include <glm/glm.hpp>
#include "core/utilities/constants.hpp"
#include "core/component.hpp"

namespace Omnia
{
	class OMNIA_ENGINE_API PhysicsBody : public Component
	{
	public:
		PhysicsBody()
		{
			this->type = TYPE_STRING;
			this->drag_ratio = glm::vec3(0.0001, 1.0, 0.0001);
			this->reload();
		};
		static constexpr const char* TYPE_STRING = "PhysicsBody";

		bool is_rigid_body = true;

		float mass = 1.0;
		float friction_ratio = 1.0;
		float angular_drag_ratio = 1.0;
		float angular_velocity = 0.0;
		float gravity_scale = 1.0;
		float elasticity_ratio = 0.01;
		glm::vec3 drag_ratio;
		glm::vec3 linear_velocity;
		glm::vec3 snap_direction;
		glm::vec3 up_direction;

		virtual Registerable* instance() override
		{
			PhysicsBody* clone = new PhysicsBody(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}
		virtual void deserialize(YAML::Node yaml_node);
		void accelerate(glm::vec3 acceleration, float maximum_speed);
		void accelerate(glm::vec3 direction, float acceleration, float maximum_speed);
		void accelerate_x(float acceleration, float maximum_speed);
		void accelerate_y(float acceleration, float maximum_speed);
		void accelerate_z(float acceleration, float maximum_speed);
		void decelerate(float deceleration);
		void decelerate_x(float deceleration);
		void decelerate_y(float deceleration);
		void decelerate_z(float deceleration);
		void move(glm::vec3 linear_velocity, glm::vec3 snap_direction, glm::vec3 up_direction);
		void reload();
	private:
	};
}