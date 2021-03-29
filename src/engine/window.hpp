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

#include <SDL.h>
#include "utilities/collections/vector.hpp"
#include "utilities/collections/map.hpp"

namespace Lilliputian
{
	class Window
	{
	public:
		Window(const char* title, uint16_t width, uint16_t height, bool isFullscreen);
		void setToWindowed(uint16_t width_px, uint16_t height_px);
		void setToFullscreen(SDL_DisplayMode* mode);
		void toggleWindowedFullscreen();
		void resizeWindow(uint16_t width_px, uint16_t height_px);
		void changeTitle(const char* title);
		void hide();
		void show();
		void sleep(int time_ms);

		SDL_Window* getSDLWindow();
	private:
		SDL_Window* sdlWindow;
		bool isFullscreen;
	};
}