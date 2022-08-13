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

#include "cpp_script_assembly.hpp"
#include "cpp_script_registry.hpp"
#include "systems/scripting_system/script_context.hpp"
#include "os/os.hpp"

void loadScriptInstances()
{
	Omnia::CPPScriptRegistry::loadScriptInstances();
}

void onStart()
{
	for (auto it : Omnia::ScriptContext::getScene()->getSceneTrees())
		executeQueuedMethods(it.second->getStartEntityQueue(), it.second, "onStart");
}

void onInput()
{
	for (auto it : Omnia::ScriptContext::getScene()->getSceneTrees())
		executeUpdateMethods(it.second, "onInput");
}

void onLogic()
{
	for (auto it : Omnia::ScriptContext::getScene()->getSceneTrees())
		executeUpdateMethods(it.second, "onLogicFrame");
}

void onCompute()
{
	for (auto it : Omnia::ScriptContext::getScene()->getSceneTrees())
		executeUpdateMethods(it.second, "onComputeFrame");
}

void onOutput()
{
	for (auto it : Omnia::ScriptContext::getScene()->getSceneTrees())
		executeUpdateMethods(it.second, "onOutput");
}

void onFinish()
{
	for (auto it : Omnia::ScriptContext::getScene()->getSceneTrees())
		executeQueuedMethods(it.second->getFinishEntityQueue(), it.second, "onFinish");
}

void bindAndCall(std::shared_ptr<Omnia::ScriptCollection> scriptCollection,
	Omnia::SceneTreeID sceneTreeID,
	Omnia::EntityID entityID,
	std::string methodName)
{
	for (auto script : scriptCollection->scripts)
	{
		std::string scriptInstanceName = script->getName() + std::to_string(entityID);
		std::unordered_map<std::string, std::shared_ptr<Omnia::CPPScript>> scriptInstances = Omnia::CPPScriptRegistry::getScriptInstances();

		if (scriptInstances.count(scriptInstanceName))
		{
			std::shared_ptr<Omnia::CPPScript> scriptInstance = scriptInstances.at(scriptInstanceName);

			Omnia::ScriptContext::bindEntity(
				sceneTreeID,
				entityID);

			if (methodName == "onStart")
				scriptInstance->onStart();
			else if (methodName == "onInput")
				scriptInstance->onInput();
			else if (methodName == "onLogicFrame")
				scriptInstance->onLogic();
			else if (methodName == "onComputeFrame")
				scriptInstance->onCompute();
			else if (methodName == "onOutput")
				scriptInstance->onOutput();
			else if (methodName == "onFinish")
				scriptInstance->onFinish();
		}
	}
}

void executeQueuedMethods(
	std::queue<Omnia::EntityID> entityQueue,
	std::shared_ptr<Omnia::SceneTree> sceneTree,
	std::string methodName)
{
	while (!entityQueue.empty())
	{
		std::shared_ptr<Omnia::Entity> entity = sceneTree->getEntity(entityQueue.front());
		std::shared_ptr<Omnia::ScriptCollection> scriptCollection = sceneTree->getComponentByType<Omnia::ScriptCollection>(entity->getID());
		if (scriptCollection != nullptr)
		{
			bindAndCall(scriptCollection, sceneTree->getID(), scriptCollection->getEntityID(), methodName);
		}
		entityQueue.pop();
	}
}

void executeUpdateMethods(
	std::shared_ptr<Omnia::SceneTree> sceneTree,
	std::string methodName)
{
	for (auto scriptCollection : sceneTree->getComponentsByType<Omnia::ScriptCollection>())
		bindAndCall(scriptCollection, sceneTree->getID(), scriptCollection->getEntityID(), methodName);
}