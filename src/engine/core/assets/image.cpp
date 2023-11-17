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

#include "image.hpp"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

Omnia::Image::Image(std::string text, std::shared_ptr<Font> font, std::shared_ptr<Colour> colour, uint16_t wrap_length)
{
	this->type = TYPE_STRING;
	this->alignment = Alignment::TOP_LEFT;

	if (text == "")
		text = " ";

	SDL_Color sdl_color = { colour->get_red(), colour->get_green(), colour->get_blue(), colour->get_alpha() };
	std::shared_ptr<SDL_Surface> sdl_surface(SDL_ConvertSurfaceFormat(TTF_RenderUTF8_Blended_Wrapped(font->get_sdlttffont(), text.c_str(), sdl_color, wrap_length), SDL_PIXELFORMAT_RGBA32, 0), SDL_FreeSurface);

	/*	SDL vertical pixel flip solution by 
		vvanpelt on StackOverflow: https://stackoverflow.com/a/65817254 
	*/
	SDL_Surface* flipped_sdlsurface = sdl_surface.get();
	SDL_LockSurface(flipped_sdlsurface);

	int pitch = flipped_sdlsurface->pitch; // row size
	char* temp = new char[pitch]; // intermediate buffer
	char* pixels = (char*)flipped_sdlsurface->pixels;

	for (int i = 0; i < flipped_sdlsurface->h / 2; ++i) {
		// get pointers to the two rows to swap
		char* row1 = pixels + i * pitch;
		char* row2 = pixels + (flipped_sdlsurface->h - i - 1) * pitch;

		// swap rows
		memcpy(temp, row1, pitch);
		memcpy(row1, row2, pitch);
		memcpy(row2, temp, pitch);
	}

	delete[] temp;

	SDL_UnlockSurface(flipped_sdlsurface);

	/////////

	if (sdl_surface != nullptr)
		this->set_to_parameters(sdl_surface->format->BytesPerPixel, sdl_surface->w, sdl_surface->h, (uint8_t*)sdl_surface->pixels);
}

Omnia::Image::Image(std::shared_ptr<Colour> colour)
{
	int width = 256;
	int height = 256;
	this->alignment = Alignment::CENTRE;
	this->set_to_colour(colour, width, height);
}

Omnia::Image::Image(std::shared_ptr<Colour> colour, int width, int height)
{
	this->alignment = Alignment::CENTRE;
	this->set_to_colour(colour, width, height);
}

Omnia::Image::Image(uint8_t* data, int width, int height, int colour_channels)
{
	this->type = TYPE_STRING;
	this->alignment = Alignment::CENTRE;
	this->set_to_parameters(colour_channels, width, height, data);
}

Omnia::Image::Image(std::string filepath)
{
	stbi_set_flip_vertically_on_load(0);
	this->type = TYPE_STRING;
	this->alignment = Alignment::CENTRE;
	this->set_name(filepath);

	std::string delimitter = "Image::";
	size_t delimitter_position = filepath.find(delimitter);
	size_t position = delimitter_position + delimitter.length();

	if (delimitter_position < filepath.length())
	{
		std::string token = filepath.substr(position, filepath.length());

		if (token == "default")
			this->set_to_default();
		if (token[0] == '#')
			this->set_to_colour(std::shared_ptr<Colour>(new Colour(token.substr(1, token.length()))), 256, 256);
	}
	else
	{
		this->data = std::shared_ptr<uint8_t>(stbi_load(filepath.c_str(), &this->width, &this->height, &this->colour_channels, 0), stbi_image_free);
	}
}

uint8_t* Omnia::Image::get_data()
{
	if (this->data != nullptr)
		return this->data.get();
	else
		return nullptr;
}

uint32_t Omnia::Image::get_width()
{
	if (this->data != nullptr)
		return this->width;
	else
		return 0;
}

uint32_t Omnia::Image::get_height()
{
	if (this->data != nullptr)
		return this->height;
	else
		return 0;
}

uint32_t Omnia::Image::get_depth()
{
	return this->get_bytes_per_pixel() * 8;
}

uint32_t Omnia::Image::get_pitch()
{
	return this->get_bytes_per_pixel() * this->get_width();
}

glm::vec2 Omnia::Image::get_dimensions()
{
	return glm::vec2(this->get_width(), this->get_height());
}

uint8_t Omnia::Image::get_bytes_per_pixel()
{
	return this->colour_channels;
}

Omnia::Image::Alignment Omnia::Image::get_alignment()
{
	return this->alignment;
}

void Omnia::Image::normal_blend(
	uint8_t* lower_image_data,
	glm::vec2 lower_image_position,
	glm::vec2 lower_image_dimensions,
	uint8_t* upper_image_data,
	glm::vec2 upper_image_position,
	glm::vec2 upper_image_dimensions)
{
	const uint8_t colour_channels = 4;

	for (int y = 0; y < upper_image_dimensions.y; y++)
	{
		for (int x = 0; x < upper_image_dimensions.x; x++)
		{
			for (int colour_channel = 0; colour_channel < colour_channels; colour_channel++)
			{
				int lower_image_index = ((y + upper_image_position.y) * upper_image_dimensions.x * colour_channels) +
					((x + upper_image_position.x) * colour_channels) + colour_channel;
				int upper_image_index = (y * upper_image_dimensions.x * colour_channels) +
					(x * colour_channels) + colour_channel;

				lower_image_data[lower_image_index] = upper_image_data[upper_image_index];
			}
		}
	}
}

void Omnia::Image::colour_pixel(uint32_t fill_colour, int x, int y)
{
	for (int colour_channel = 0; colour_channel < this->colour_channels; colour_channel++)
	{
		const int colour_channel_byte_length = 8;
		int image_index = (y * this->width * this->colour_channels) + (x * this->colour_channels) + colour_channel;
		int colour_byte_length = this->colour_channels * colour_channel_byte_length;
		int colour_channel_byte_offset = colour_channel * colour_channel_byte_length;
		int mask_byte_shift = colour_byte_length - colour_channel_byte_length - colour_channel_byte_offset;
		uint8_t colour_channel_value = (uint8_t)((fill_colour & (0x000000FF << mask_byte_shift)) >> mask_byte_shift);
		this->data.get()[image_index] = colour_channel_value;
	}
}

void Omnia::Image::set_to_default()
{
	const uint32_t lighter_grey = 0x333333;
	const uint32_t darker_grey = 0x2d2d2d;
	const uint16_t size = 256;
	const uint8_t divisions = 4;
	this->height = size;
	this->width = size;
	this->colour_channels = 3;
	size_t data_size = this->width * this->height * this->colour_channels;

	this->data = std::shared_ptr<uint8_t>(new uint8_t[data_size]);
	uint32_t fill_colour = 0;
	bool darker = false;

	for (int y = 0; y < this->height; y++)
	{
		for (int x = 0; x < this->width; x++)
		{
			if (darker)
				fill_colour = darker_grey;
			else
				fill_colour = lighter_grey;

			this->colour_pixel(fill_colour, x, y);

			if ((x % (size / divisions)) == 0)
				darker = !darker;
		}

		if ((y % (size / divisions)) == 0)
			darker = !darker;
	}
}

void Omnia::Image::set_to_colour(std::shared_ptr<Colour> colour, int width, int height)
{
	this->type = TYPE_STRING;
	this->width = width;
	this->height = height;
	this->colour_channels = 4;
	size_t data_size = this->width * this->height * this->colour_channels;
	const uint32_t fill_colour = colour->get_rgba();
	this->data = std::shared_ptr<uint8_t>(new uint8_t[data_size]);

	for (int y = 0; y < this->height; y++)
		for (int x = 0; x < this->width; x++)
			this->colour_pixel(fill_colour, x, y);
}

void Omnia::Image::set_to_parameters(int colour_channels, int width, int height, uint8_t* data)
{
	this->height = height;
	this->width = width;
	this->colour_channels = colour_channels;
	size_t data_size = this->width * this->height * this->colour_channels;
	const int colour_channel_byte_length = 8;
	this->data = std::shared_ptr<uint8_t>(new uint8_t[data_size]);

	for (int y = 0; y < this->height; y++)
	{
		for (int x = 0; x < this->width; x++)
		{
			uint32_t fill_colour = 0;
			for (int colour_channel = 0; colour_channel < colour_channels; colour_channel++)
				fill_colour |= data[(y * this->width * this->colour_channels) + (x * this->colour_channels) + colour_channel] <<
								(colour_channel_byte_length * (colour_channels - 1 - colour_channel));

			this->colour_pixel(fill_colour, x, y);
		}
	}
}