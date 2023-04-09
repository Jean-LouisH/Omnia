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

#include "core/asset.hpp"
#include <stdint.h>
#include <string>
#include <engine_api.hpp>
#include <glm/glm.hpp>

namespace Omnia
{
	class OMNIA_ENGINE_API Colour : public Asset
	{
	public:
		static constexpr const char* TYPE_STRING = "Colour";

		Colour()
		{
			this->type = TYPE_STRING;
		};
		Colour(std::string hex);
		Colour(uint32_t value);
		Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
		Colour(double red, double green, double blue, double alpha);

		virtual Registerable* instance() override
		{
			Colour* clone = new Colour(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}

		uint8_t getRed();
		uint8_t getGreen();
		uint8_t getBlue();
		uint8_t getAlpha();
		uint32_t getRGB();
		uint32_t getRGBA();
		glm::vec4 getRGBAInVec4();
	private:
		float red = 1.0;
		float green = 1.0;
		float blue = 1.0;
		float alpha = 1.0;

		void setRGB(uint32_t value);
		void setRGBA(uint32_t value);
	};
}