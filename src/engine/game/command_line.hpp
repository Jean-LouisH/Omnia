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
#include "utilities/collections/map.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/collections/stack.hpp"
#include "scene_serializer.hpp"
#include "os/os.hpp"
#include "os/profiler.hpp"
#include "scene_storage.hpp"

namespace Lilliputian
{
	class CommandLine
	{
	public:
		CommandLine(
			Vector<String>* scripts,
			SceneSerializer* sceneSerializer,
			SceneStorage* sceneStorage
			);

		void execute(String command);
	private:
		Vector<String>* scripts = nullptr;
		Vector<Scene>* loadedScenes = nullptr;
		Stack<Scene>* activeSceneStack = nullptr;
		SceneSerializer* sceneSerializer = nullptr;
		SceneIndex* activeSceneIndex = nullptr;
		Profiler* profiler = nullptr;
	};
}