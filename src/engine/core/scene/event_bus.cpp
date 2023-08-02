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

#include "event_bus.hpp"
#include <core/singletons/os/os.hpp>

void Omnia::EventBus::publish(
	std::string name,
	std::unordered_map<std::string, double> numbers,
	std::unordered_map<std::string, std::string> strings)
{
	Event::Parameters parameters = {numbers, strings};
	this->publishWithParameters(name, parameters);
}

void Omnia::EventBus::publish(
	std::string name,
	std::unordered_map<std::string, double> numbers)
{
	Event::Parameters parameters;
	parameters.numbers = numbers;
	this->publishWithParameters(name, parameters);
}

void Omnia::EventBus::publish(
	std::string name,
	std::unordered_map<std::string, std::string> strings)
{
	Event::Parameters parameters;
	parameters.strings = strings;
	this->publishWithParameters(name, parameters);
}

void Omnia::EventBus::publish(
	std::string name)
{
	std::vector<Event> eventsList;

	OS::getRunTimer().setEnd();

	if (this->updateEvents.count(name))
		eventsList = this->updateEvents.at(name);

	eventsList.push_back(Event(
		name,
		OS::getRunTimer().getDeltaInNanoseconds()));

	this->updateEvents.emplace(name, eventsList);
	this->outputEvents.emplace(name, eventsList);
}

void Omnia::EventBus::clear()
{
	this->updateEvents.clear();
}

void Omnia::EventBus::clearOutputEvents()
{
	this->outputEvents.clear();
}

std::vector<Omnia::Event> Omnia::EventBus::query(std::string name)
{
	std::vector<Event> queryResults;

	if (this->updateEvents.count(name))
		queryResults = this->updateEvents.at(name);

	return queryResults;
}

std::vector<Omnia::Event> Omnia::EventBus::queryOutputEvents(std::string name)
{
	std::vector<Event> queryResults;

	if (this->outputEvents.count(name))
		queryResults = this->outputEvents.at(name);

	return queryResults;
}

uint64_t Omnia::EventBus::queryCount(std::string name)
{
	return this->query(name).size();
}

uint64_t Omnia::EventBus::queryOutputEventCount(std::string name)
{
	return this->queryOutputEvents(name).size();
}

void Omnia::EventBus::publishWithParameters(std::string name, Event::Parameters parameters)
{
	std::vector<Event> eventsList;

	OS::getRunTimer().setEnd();

	if (this->updateEvents.count(name))
		eventsList = this->updateEvents.at(name);

	eventsList.push_back(Event(
		name,
		OS::getRunTimer().getDeltaInNanoseconds(),
		parameters));

	if (this->updateEvents.count(name))
	{
		this->updateEvents.at(name) = eventsList;
	}
	else
	{
		this->updateEvents.emplace(name, eventsList);
		this->outputEvents.emplace(name, eventsList);
	}
}