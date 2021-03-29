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

#include <stdint.h>
#include "scene/scene_forest.hpp"
#include "os.hpp"
#include "file_access.hpp"
#include "profiler.hpp"
#include "script_registry.hpp"
#include "scripting_apis/scripting_apis.hpp"
#include "scene_serializer.hpp"
#include "boot_configuration.hpp"
#include "command_line.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/collections/stack.hpp"
#include "utilities/collections/map.hpp"
#include "utilities/string.hpp"

namespace Lilliputian
{
	class Game
	{
	public:
		Game(
			OS* os,
			Profiler* profiler);
		void initialize();
		void executeOnStartMethods();
		void executeOnInputMethods();
		void executeOnFrameMethods();
		void executeOnComputeMethods();
		void executeOnLateMethods();
		void executeOnFinalMethods();
		void deinitialize();
		ScriptRegistry& getScriptRegistry();
		SceneForest& getActiveScene();
		BootConfiguration& getConfiguration();
	private:
		OS* os = nullptr;
		Profiler* profiler = nullptr;
		BootConfiguration* configuration = nullptr;
		ScriptingAPIs* scriptingAPIs = nullptr;
		ScriptRegistry* scriptRegistry = nullptr;
		SceneSerializer* sceneSerializer = nullptr;
		CommandLine* commandLine = nullptr;
		Map<String, Script> scripts;
		Vector<SceneForest> preloadedScenes;
		Stack<SceneForest> activeSceneStack;
	};
}