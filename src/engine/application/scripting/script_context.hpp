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

#include "application/scene/scene.hpp"
#include <application/scene_storage.hpp>
#include <application/configuration.hpp>
#include "utilities/aliases.hpp"
#include "os/os.hpp"
#include <string>

#include <memory>
#include <application/scene/component.hpp>
#include <application/scene/scene.hpp>
#include <application/scene_serializer.hpp>
#include <application/scene_storage.hpp>
#include <utilities/aliases.hpp>
#include <application/scene/event_bus.hpp>

namespace Omnia
{
	class ScriptContext
	{
	public:
		static void setSceneStorage(SceneStorage* sceneStorage);
		static void bindEntity(SceneTreeID sceneTreeID, EntityID entityID);

		static std::shared_ptr<Asset> loadAsset(std::string type, std::string filepath);
		static std::shared_ptr<Image> loadImage(std::string filepath);

		static bool hasComponent(std::string type);
		static Entity& getEntity();
		static Scene& getScene();
		static SceneTree& getSceneTree();
		static Component& getComponent(std::string type);

		static ScriptContext* getInstance();
	private:
		static ScriptContext* instance;

		SceneTreeID boundSceneTreeID = 0;
		EntityID boundEntityID = 0;
		SceneStorage* sceneStorage = nullptr;
	};
}
