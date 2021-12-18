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

#include "font.hpp"
#include <application/scene/id_counter.hpp>


Esi::Font::Font()
{
	this->font = nullptr;
}

Esi::Font::Font(const char* filepath, uint16_t size_px)
{
	this->font = TTF_OpenFont(filepath, size_px);
	this->id = IDCounter::getNewAssetID();
}

Esi::Font::Font(TTF_Font* font)
{
	this->font = font;
	this->id = IDCounter::getNewAssetID();
}

TTF_Font* Esi::Font::getSDLTTFFont()
{
	return this->font;
}

Esi::AssetID Esi::Font::getID()
{
	return this->id;
}

void Esi::Font::unload()
{
	TTF_CloseFont(this->font);
	this->font = nullptr;
}