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

#include "scene.hpp"
#include "component.hpp"
#include "application/scripting/python_vm/script_call_batch.hpp"

Omnific::Scene::Scene()
{
	this->assetCache = std::shared_ptr<AssetCache>(new AssetCache());
	this->eventBus = std::shared_ptr<EventBus>(new EventBus());
	this->hapticSignalBuffer = std::shared_ptr<HapticSignalBuffer>(new HapticSignalBuffer());
	this->componentPropertyPool = std::shared_ptr<ComponentPropertyPool>(new ComponentPropertyPool());

	std::shared_ptr<Transform> transform(new Transform());
	std::shared_ptr<Component> component = std::static_pointer_cast<Component>(transform);

	this->ID = UIDGenerator::getNewUID();
	this->addEmptyEntity();
	this->addComponentToLastEntity(component);
	this->dummyEntityID = this->getLastEntity().id;
}

void Omnific::Scene::addEntity(Entity entity)
{
	if (entity.parentID != DUMMY_ENTITY)
		this->entities.at(entity.parentID).childIDs.push_back(entity.id);

	this->startEntitiesQueue.emplace(entity.id);
	this->entities.emplace(entity.id, entity);
	this->lastEntityID = entity.id;
}

void Omnific::Scene::addEmptyEntity()
{
	Entity emptyEntity;
	this->addEntity(emptyEntity);
}

void Omnific::Scene::addComponent(EntityID entityID, std::shared_ptr<Component> component)
{
	component->setEntityID(entityID);
	component->setComponentPropertyPool(this->componentPropertyPool);
	this->components.push_back(component);
	std::string type = component->getType();
	this->entities.at(entityID).components.emplace(type, component->getID());
	size_t lastIndex = this->components.size() - 1;

	if (this->componentIndexCaches.count(type) > 0)
	{
		this->componentIndexCaches.at(type).push_back(lastIndex);
	}
	else
	{
		std::vector<size_t> componentIndices;
		componentIndices.push_back(lastIndex);
		this->componentIndexCaches.emplace(type, componentIndices);
	}

	if (component->isRenderable())
		this->renderOrderIndexCache.push_back(lastIndex);
}

void Omnific::Scene::addComponentToLastEntity(std::shared_ptr<Component> component)
{
	this->addComponent(this->lastEntityID, component);
}

void Omnific::Scene::removeEntity(EntityID entityID)
{
	auto entityComponents = this->getEntity(entityID).components;
	for (auto it = entityComponents.begin(); it != entityComponents.end();)
		this->removeComponent(entityID, it->first);


	std::vector<EntityID> childIDs = this->getEntity(entityID).childIDs;

	for (int i = 0; i < childIDs.size(); i++)
		this->getEntity(childIDs.at(i)).parentID = DUMMY_ENTITY;

	EntityID parentID = this->getEntity(entityID).parentID;
	std::vector<EntityID> parentChildIDs = this->getEntity(parentID).childIDs;
}

void Omnific::Scene::removeComponent(EntityID entityID, std::string type)
{
	Entity& entity = this->getEntity(entityID);
	
	if (entity.components.count(type) > 0)
	{
		ComponentID componentID = entity.components.at(type);
		entity.components.erase(type);

		/* Remove the component from the list. */

		for (auto it = this->components.begin(); it != this->components.end();)
		{
			if ((*it)->getID() == componentID)
			{
				it = this->components.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}

		/* Rebuild index caches */

		this->componentIndexCaches.clear();
		this->renderOrderIndexCache.clear();

		for (size_t i = 0; i < components.size(); i++)
		{
			std::shared_ptr<Component> component = components.at(i);
			this->componentIndexCaches.at(component->getType()).push_back(i);
			if (component->isRenderable())
				this->renderOrderIndexCache.push_back(i);
		}
	}
}

std::vector<Omnific::ScriptCallBatch> Omnific::Scene::generateCallBatches(CallType callType)
{
	std::vector<ScriptCallBatch> scriptCallBatches;
	std::queue<EntityID>* entityQueue = nullptr;

	if (callType == CallType::START || callType == CallType::FINISH)
	{
		if (callType == CallType::START)
			entityQueue = &this->startEntitiesQueue;
		else if (callType == CallType::FINISH)
			entityQueue = &this->finishEntitiesQueue;

		for (auto it = this->entities.begin(); it != this->entities.end(); it++)
		{
			Entity entity = it->second;
			if (!entityQueue->empty())
			{
				if (entity.id == entityQueue->front())
				{
					scriptCallBatches.push_back({ entity.scripts, this->ID, entity.id });
					entityQueue->pop();
				}
			}
			else
			{
				break;
			}
		}
	}
	else if (callType == CallType::UPDATE)
	{
		for (auto it = this->entities.begin(); it != this->entities.end(); it++)
		{
			Entity entity = it->second;
			scriptCallBatches.push_back({ entity.scripts, this->ID, entity.id });
		}
	}

	return scriptCallBatches;
}

std::vector<size_t> Omnific::Scene::getRenderOrderIndexCache()
{
	return this->renderOrderIndexCache;
}

std::unordered_map<std::string, std::vector<size_t>> Omnific::Scene::getComponentIndexCaches()
{
	return this->componentIndexCaches;
}

std::vector<std::shared_ptr<Omnific::Component>> Omnific::Scene::getComponents()
{
	return this->components;
}

std::shared_ptr<Omnific::Transform> Omnific::Scene::getEntityTransform(EntityID entityID)
{
	/* Gets the dummy root Entity Transform by default. */
	std::shared_ptr<Transform> transform = this->getComponentsByType<Transform>().at(0);
	Entity& entity = this->getEntity(entityID);

	if (entity.components.count(Transform::TYPE_STRING) > 0)
	{
		std::shared_ptr<Component> transformComponent = this->getComponent(entity.components.at(Transform::TYPE_STRING));
		transform = std::dynamic_pointer_cast<Transform>(transformComponent);
	}

	return transform;
}

Omnific::Entity& Omnific::Scene::getEntity(EntityID entityID)
{
	return this->entities.at(entityID);
}

Omnific::Entity& Omnific::Scene::getEntityByName(std::string name)
{
	Entity* Entity = nullptr;

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
		if (it->second.name == name)
			return it->second;

	return *Entity;
}

Omnific::Entity& Omnific::Scene::getLastEntity()
{
	return this->entities.at(this->lastEntityID);
}

Omnific::EntityID Omnific::Scene::getDummyEntityID()
{
	return this->dummyEntityID;
}

std::unordered_map<Omnific::EntityID, Omnific::Entity>& Omnific::Scene::getEntities()
{
	return this->entities;
}

std::shared_ptr<Omnific::Component> Omnific::Scene::getComponent(ComponentID componentID)
{
	std::shared_ptr<Component> component = std::make_shared<Component>();

	for (int i = 0; i < this->components.size(); i++)
	{
		std::shared_ptr<Component> currentComponent = this->components.at(i);
		if (currentComponent->getID() == componentID)
			component = currentComponent;
	}

	return component;
}

Omnific::Entity::SpatialDimension Omnific::Scene::getComponentSpatialDimension(ComponentID componentID)
{
	return this->getEntity(this->getComponent(componentID)->getEntityID()).spatialDimension;
}

Omnific::EventBus& Omnific::Scene::getEventBus()
{
	return *this->eventBus;
}

bool Omnific::Scene::getHasRenderableComponentsChanged()
{
	return true;
}

bool Omnific::Scene::getHasShadersChanged()
{
	return false;
}

bool Omnific::Scene::getHasScriptsChanged()
{
	return false;
}

Omnific::SceneID Omnific::Scene::getID()
{
	return this->ID;
}

Omnific::AssetCache& Omnific::Scene::getAssetCache()
{
	return *this->assetCache;
}

Omnific::HapticSignalBuffer& Omnific::Scene::getHapticSignalBuffer()
{
	return *this->hapticSignalBuffer;
}

void Omnific::Scene::unload()
{
	this->assetCache->deleteAllAssets();
}

Omnific::Entity& Omnific::Scene::getDummyEntity()
{
	return this->getEntity(this->dummyEntityID);
}