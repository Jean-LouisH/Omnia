#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Collections/Vector.hpp"
#include "../../../Utilities/Collections/Map.hpp"
#include "../../../Utilities/Constants.hpp"
#include "../../../Utilities/String.hpp"
#include "../Assets/Texture.hpp"

namespace Lilliputian
{
	class AnimatedSprite2D
	{
		using Frame = Texture;
		using FrameIndex = uint16_t;
		using FrameSequence = Vector<Frame>;
	private:
		Map<String, FrameSequence> frameSequences;
		uint8_t alpha = 255;
		float animationSpeed_fps = 12.0;
		float frameTime_s = 0.0;
		FrameIndex currentFrameIndex = 0;
		bool isPlaying = false;
		bool isBackwards = false;
		bool isFlippedVertically = false;
		bool isFlippedHorizontally = false;
	public:
		EntityID entityID = NO_ENTITY;
		String currentFrameSequenceName = "";

		void addEmptyFrameSequence(String frameSequenceName);
		void addFrameSequence(String frameSequenceName, FrameSequence frameSequence);
		void addFrameToFrameSequence(String frameSequenceName, Frame frame);
		void clearFrameSequences();

		void setAlpha(uint8_t value);
		uint8_t getAlpha();
		void hide();
		void show();

		void setAnimationSpeed(float value_fps);
		float getAnimationSpeed();

		void update(float delta_s);

		void play(String frameSequenceName);
		void play();
		void pause();
		void stop();

		void setBackwards();
		void setForwards();

		void flipVertically();
		void flipHorizontally();

		Frame getCurrentFrame();
		Vector<String> getFrameSequenceNames();
		FrameSequence getFrameSequenceByName(String frameSequenceName);
		FrameSequence getCurrentFrameSequence();
	};
}