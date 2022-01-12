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

#include "utilities/aliases.hpp"
#include "utilities/constants.hpp"
#include "application/scene/component.hpp"

#include "utilities/aabb_2d.hpp"


namespace Esi
{
	class Collider : public Component
	{
	public:
		struct Circle
		{
			float radius = 0.0;
		}circle;

		struct Box
		{
			enum PlatformSide
			{
				PLATFORM_SIDE_NONE,
				PLATFORM_SIDE_LEFT,
				PLATFORM_SIDE_RIGHT,
				PLATFORM_SIDE_TOP,
				PLATFORM_SIDE_BOTTOM
			};

			AABB2D aabb;
			PlatformSide platformSide = PLATFORM_SIDE_NONE;
		}box;



		Collider()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "Collider";
	private:
	};
}