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

#include "imgui_test.hpp"

void Sample::IMGUITest::onStart()
{
    std::shared_ptr<Omnia::Timer> timer = Omnia::EntityContext::getComponentByType<Omnia::Timer>();
    float countdownValue = 5.0f;
    timer->start(countdownValue);
}

void Sample::IMGUITest::onLogic()
{
    std::shared_ptr<Omnia::Timer> timer = Omnia::EntityContext::getComponentByType<Omnia::Timer>();

    if (timer->isFinished())
    {
        timer->start(1.0f);
        timer->stop();
        Omnia::OS::getLogger().write("Time's up!");
    }
}