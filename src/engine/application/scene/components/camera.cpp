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

#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <os/os.hpp>

void Omnia::Camera::resetAspect()
{
	this->aspect = defaultAspect;
}

void Omnia::Camera::toggleWireframeMode()
{
	if (this->getIsWireframeMode())
		this->setWireframeMode(false);
	else
		this->setWireframeMode(true);
}

Omnia::Rectangle Omnia::Camera::getViewportDimensions()
{
	return this->viewport;
}

bool Omnia::Camera::getIsStreaming()
{
	return this->isStreaming;
}

bool Omnia::Camera::getIsWireframeMode()
{
	return this->isWireFrame;
}

void Omnia::Camera::setViewportWidth(uint32_t width_px)
{
	this->viewport.width = width_px;
	if (keepAspect)
		this->viewport.height = (uint32_t)((double)width_px / aspect);
}

void Omnia::Camera::setViewportHeight(uint32_t height_px)
{
	this->viewport.height = height_px;
	if (keepAspect)
		this->viewport.width = (uint32_t)((double)height_px * aspect);
}

void Omnia::Camera::setLimits(float left, float bottom, float top, float right)
{
	this->limits.min.x = left;
	this->limits.min.y = bottom;
	this->limits.max.x = right;
	this->limits.max.y = top;
}

void Omnia::Camera::setKeepAspect(bool value)
{
	this->keepAspect = value;
}

void Omnia::Camera::setIsStreaming(bool value)
{
	this->isStreaming = value;
}

void Omnia::Camera::setWireframeMode(bool value)
{
	this->isWireFrame = value;
}

glm::mat4 Omnia::Camera::getViewToProjectionMatrix()
{
	Rectangle rectangle = OS::getWindow().getWindowSize();
	return glm::perspective(glm::radians(this->fieldOfView), (float)rectangle.width / (float)rectangle.height, this->nearPlane, this->farPlane);
}