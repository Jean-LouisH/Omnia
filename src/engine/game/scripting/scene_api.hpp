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

#include "utilities/string.hpp"
#include <game/scene/components/component_variant.hpp>
#include <game/scene/scene.hpp>
#include <game/scene_serializer.hpp>
#include <utilities/aliases.hpp>

namespace Lilliputian
{
	class SceneAPI
	{
	public:
		void bindScene(Scene* scene);
		void bindEntity(SceneTreeID sceneTreeID, EntityID entityID);
		bool hasComponent(String typeString);
		void changeToScene(String sceneFilename);
		void setSceneSerializer(SceneSerializer* sceneSerializer);

		Entity2D& getThisEntity2D();
		SceneTree2D& getThisSceneTree2D();
		Scene& getScene();
		ComponentVariant& getComponentVariant(ComponentVariant::Type type);
		UITextLabel& getUITextLabel();
	private:
		SceneTreeID boundSceneTreeID = 0;
		EntityID boundEntityID = 0;
		Scene* scene = nullptr;
		SceneSerializer* sceneSerializer = nullptr;

		ComponentVariant::Type convertStringToType(String typeString);
	};
}