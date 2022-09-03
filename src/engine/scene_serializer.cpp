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

#include "scene_serializer.hpp"
#include <yaml-cpp/yaml.h>
#include <string>
#include <set>
#include "scene/scene.hpp"
#include "scene/components/component.hpp"
#include <os/os.hpp>
#include "asset_pipeline/asset_pipeline.hpp"

#include <customization/class_registry/class_registry.hpp>

Omnia::SceneSerializer* Omnia::SceneSerializer::instance = nullptr;

void Omnia::SceneSerializer::initialize(std::string dataDirectory)
{
	SceneSerializer::getInstance()->dataDirectory = dataDirectory;
}

bool Omnia::SceneSerializer::doesSceneExist(std::string filepath)
{
	return OS::getFileAccess().exists(SceneSerializer::getInstance()->dataDirectory + filepath);
}

void Omnia::SceneSerializer::serialize(std::string filepath, std::shared_ptr<Scene> scene)
{

}

std::shared_ptr<Omnia::Scene> Omnia::SceneSerializer::deserialize(std::string filepath)
{
	return SceneSerializer::getInstance()->deserialize(filepath, "");
}

std::shared_ptr<Omnia::Scene> Omnia::SceneSerializer::deserialize(std::string filepath, std::string name)
{
	std::shared_ptr<Scene> scene = std::shared_ptr<Scene>(new Scene());
	const std::string fullFilepath = SceneSerializer::getInstance()->dataDirectory + filepath;

	try
	{
		YAML::Node yamlNode = YAML::LoadFile(fullFilepath);

		for (YAML::const_iterator it0 = yamlNode.begin(); it0 != yamlNode.end(); ++it0)
		{
			if (it0->first.as<std::string>() == "SceneTree")
			{
				std::shared_ptr<SceneTree> sceneTree(new SceneTree());
				bool loadThisSceneTree = true;

				/* If the name is an empty string, load all, otherwise search for the name */
				if (name != "")
				{
					loadThisSceneTree = false;

					for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
					{
						if (it1->first.as<std::string>() == "name")
						{
							loadThisSceneTree = it1->second.as<std::string>() == name;
							break;
						}
					}
				}

				if (loadThisSceneTree)
				{
					for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
					{
						if (it1->first.as<std::string>() == "name")
						{
							sceneTree->name = it1->second.as<std::string>();
						}
						else if (it1->first.as<std::string>() == "spatial_dimension")
						{
							int value = it1->second.as<int>();

							if (value == 2)
								sceneTree->is2D = true;
							else if (value == 3)
								sceneTree->is2D = false;
						}
						else if (it1->first.as<std::string>() == "Entity")
						{
							std::shared_ptr<Entity> entity(new Entity());
							sceneTree->addEntity(entity);

							for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
							{
								//Entity attributes
								if (it2->first.as<std::string>() == "name")
								{
									sceneTree->setEntityName(sceneTree->getLastEntity()->getID(), it2->second.as<std::string>());
								}
								else if (it2->first.as<std::string>() == "parent")
								{
									sceneTree->getLastEntity()->parentID = sceneTree->getEntityByName(it2->second.as<std::string>())->getID();
								}
								//Components
								else
								{
									std::shared_ptr<Component> component = std::dynamic_pointer_cast<Component>(
										std::shared_ptr<Registerable>(ClassRegistry::query<Component>(it2->first.as<std::string>())->copy()));
									component->deserialize(it2->second);
									sceneTree->addComponentToLastEntity(component);
								}
							}
						}
						/* Recursively load another SceneTree into this one if the filename
						   is not the same. */
						else if (it1->first.as<std::string>() == "SubSceneTree")
						{
							EntityID parentID = 0;

							for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
							{
								if (it2->first.as<std::string>() == "parent")
								{
									parentID = sceneTree->getEntityByName(it2->second.as<std::string>())->getID();
								}
								else if (it2->first.as<std::string>() == "name")
								{
									std::string subSceneFilepath = it1->second[0].as<std::string>();
									std::string subSceneTreeName = it1->second[1].as<std::string>();

									if (subSceneFilepath != filepath)
									{
										std::shared_ptr<SceneTree> subSceneTree = SceneSerializer::getInstance()->deserialize(subSceneFilepath, subSceneTreeName)->getLastSceneTree();

										/* Only load the SceneTree if it is the same spatial dimension. */
										if (subSceneTree->is2D == sceneTree->is2D)
										{
											/* Transfer Entities and their Components */
											std::shared_ptr<Entity> newRootEntity;
											newRootEntity->setName(subSceneFilepath);
											newRootEntity->parentID = parentID;
											sceneTree->addEntity(newRootEntity);
											std::unordered_map<EntityID, std::shared_ptr<Entity>>& subSceneEntities = subSceneTree->getEntities();

											for (auto it = subSceneEntities.begin(); it != subSceneEntities.end(); it++)
											{
												std::shared_ptr<Entity> subSceneEntity = it->second;

												if (subSceneEntity->parentID == 0)
													subSceneEntity->parentID = newRootEntity->getID();

												sceneTree->addEntity(subSceneEntity);

												std::unordered_map<std::string, ComponentID> subSceneEntityComponentIDs = subSceneEntity->componentIDs;

												for (auto it2 = subSceneEntityComponentIDs.begin(); it2 != subSceneEntityComponentIDs.end(); it2++)
													sceneTree->addComponentToLastEntity(subSceneTree->getComponent(it2->second));
											}
										}
									}
								}
							}
						}
					}
				}

				scene->addSceneTree(sceneTree);
			}
		}
	}
	catch (int e)
	{

	}

	return scene;
}

std::string Omnia::SceneSerializer::getDataDirectory()
{
	return getInstance()->dataDirectory;
}

Omnia::SceneSerializer* Omnia::SceneSerializer::getInstance()
{
	if (instance == nullptr)
		instance = new SceneSerializer();
	return instance;
}