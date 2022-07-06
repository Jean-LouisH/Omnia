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

#include "sprite_container.hpp"

void Omnia::SpriteContainer::addImage(std::shared_ptr<Image> image)
{
	this->addEmptyFrameSequence("");
	this->addImageToFrameSequence("", image);
}

void Omnia::SpriteContainer::addEmptyFrameSequence(std::string frameSequenceName)
{
	std::vector<std::shared_ptr<Image>> frameSequence;
	this->frameSequences.emplace(frameSequenceName, frameSequence);
}

void Omnia::SpriteContainer::addFrameSequence(std::string frameSequenceName, std::vector<std::shared_ptr<Image>> frameSequence)
{
	this->frameSequences.emplace(frameSequenceName, frameSequence);
	this->image = this->getCurrentFrame();
}

void Omnia::SpriteContainer::addImageToFrameSequence(std::string frameSequenceName, std::shared_ptr<Image> frame)
{
	if (this->frameSequences.count(frameSequenceName))
	{
		std::vector<std::shared_ptr<Image>> frameSequence = this->frameSequences.at(frameSequenceName);
		this->frameSequences.at(frameSequenceName).push_back(frame);
		if (frameSequence.size() == 1)
			this->setDimensions(frame->getWidth(), frame->getHeight());
	}
	this->image = this->getCurrentFrame();
}

void Omnia::SpriteContainer::clearFrameSequences()
{
	this->frameSequences.clear();
	this->currentFrameIndex = 0;
	this->image = this->getCurrentFrame();
}

void Omnia::SpriteContainer::setAnimationSpeed(float value_fps)
{
	if (value_fps > 0.0)
		this->animationSpeedInFPS = value_fps;
}

float Omnia::SpriteContainer::getAnimationSpeed()
{
	return this->animationSpeedInFPS;
}

void Omnia::SpriteContainer::update(float delta_s)
{
	if (this->isPlaying)
	{
		this->frameTime += delta_s;

		if (this->frameTime > (1.0 / this->animationSpeedInFPS))
		{
			uint32_t currentFrameSequenceSize = this->getCurrentFrameSequence().size();

			if (!this->isBackwards)
				this->currentFrameIndex = 
				this->currentFrameIndex + 1 % currentFrameSequenceSize;
			else
				this->currentFrameIndex = 
				this->currentFrameIndex - 1 < 0 ? currentFrameSequenceSize - 1 : this->currentFrameIndex - 1;

			this->frameTime = 0.0;
		}
	}

	this->image = this->getCurrentFrame();
}

void Omnia::SpriteContainer::play(std::string frameSequenceName)
{
	if (this->frameSequences.count(frameSequenceName))
	{
		this->isPlaying = true;
		this->currentFrameSequenceName = frameSequenceName;
	}
}

void Omnia::SpriteContainer::play()
{
	this->isPlaying = true;
}

void Omnia::SpriteContainer::pause()
{
	this->isPlaying = false;
}

void Omnia::SpriteContainer::stop()
{
	this->pause();
	this->currentFrameIndex = 0;
}

void Omnia::SpriteContainer::setBackwards()
{
	this->isBackwards = true;
}
void Omnia::SpriteContainer::setForwards()
{
	this->isBackwards = false;
}

void Omnia::SpriteContainer::flipVertically()
{
	this->isFlippedVertically != this->isFlippedVertically;
}

void Omnia::SpriteContainer::flipHorizontally()
{
	this->isFlippedHorizontally != this->isFlippedHorizontally;
}

std::shared_ptr<Omnia::Image> Omnia::SpriteContainer::getCurrentFrame()
{
	std::shared_ptr<Image> image = std::shared_ptr<Image>(new Image());
	if (this->frameSequences.size() > 0)
		image = this->getCurrentFrameSequence().at(this->currentFrameIndex);
	return image;
}

std::vector<std::string> Omnia::SpriteContainer::getFrameSequenceNames()
{
	std::vector<std::string> frameSequenceNames;

	for (std::unordered_map<std::string, std::vector<std::shared_ptr<Image>>>::iterator it = this->frameSequences.begin();
		it != this->frameSequences.end();
		++it)
	{
		frameSequenceNames.push_back(it->first);
	}

	return frameSequenceNames;
}

std::vector<std::shared_ptr<Omnia::Image>> Omnia::SpriteContainer::getFrameSequenceByName(std::string frameSequenceName)
{
	std::vector<std::shared_ptr<Image>> frameSequence;

	if (this->frameSequences.count(frameSequenceName))
		frameSequence = this->frameSequences.at(frameSequenceName);

	return frameSequence;
}

std::vector<std::shared_ptr<Omnia::Image>> Omnia::SpriteContainer::getCurrentFrameSequence()
{
	return this->frameSequences.at(this->currentFrameSequenceName);
}