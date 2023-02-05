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

#include "model.hpp"
#include <core/scene/scene.hpp>
#include <core/singletons/os/os.hpp>

void Omnia::Model::deserialize(YAML::Node yamlNode)
{
	this->material = std::shared_ptr<Material>(new Material());

	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "mesh")
		{
			this->mesh = std::shared_ptr<Mesh>(new Mesh(it3->second.as<std::string>()));
		}
		else if (it3->first.as<std::string>() == "albedo")
		{
			this->material->albedo = std::shared_ptr<Image>(new Image(it3->second.as<std::string>()));
		}
	}

	if (this->mesh == nullptr)
	{
		this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::cube"));
	}

	if (this->material->albedo == nullptr)
	{
		this->material->albedo = std::shared_ptr<Image>(new Image("Image::default"));
	}
}

void Omnia::Model::setToCube()
{
	this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::cube"));
	this->material = std::shared_ptr<Material>(new Material());
	this->material->albedo = std::shared_ptr<Image>(new Image("Image::default"));
}

void Omnia::Model::setToTexturedCube(std::shared_ptr<Material> material)
{
	this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::cube"));
	this->material = material;
}

void Omnia::Model::setMesh(std::shared_ptr<Mesh> mesh)
{
	this->mesh = mesh;
}

void Omnia::Model::setMaterial(std::shared_ptr<Material> material)
{
	this->material = material;
}

void Omnia::Model::setRig(std::shared_ptr<Rig> rig)
{
	this->rig = rig;
}

void Omnia::Model::addSkeletalAnimation(std::shared_ptr<SkeletalAnimation> skeletalAnimation)
{
	this->skeletalAnimations.push_back(skeletalAnimation);
}

std::shared_ptr<Omnia::Mesh> Omnia::Model::getMesh()
{
	return this->mesh;
}

std::shared_ptr<Omnia::Material> Omnia::Model::getMaterial()
{
	return this->material;
}

std::shared_ptr<Omnia::Rig> Omnia::Model::getRig()
{
	return this->rig;
}

std::vector<std::shared_ptr<Omnia::SkeletalAnimation>> Omnia::Model::getSkeletalAnimations()
{
	return this->skeletalAnimations;
}