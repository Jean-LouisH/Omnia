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

#include "engine.hpp"

Lilliputian::Engine::Engine(
	int argc, 
	char* argv[])
{
	this->profiler = new Profiler();
	this->profiler->getRunTimer().setStart();
	this->profiler->getBenchmarkTimer().setStart();
	this->argc = argc;
	this->argv = argv;
}

void Lilliputian::Engine::run()
{
	do
	{
		if (this->initialize())
		{
			this->game->initialize();
			BootConfiguration configuration = this->game->getConfiguration();

			if (configuration.isLoaded)
			{
				this->os->addGameControllerMappings();
				Window window = this->os->getWindow();
				window.resizeWindow(configuration.windowWidth, configuration.windowHeight);
				window.changeTitle(configuration.gameTitle.c_str());
				this->state.setRunningApplicationWindowed();
			}
			else
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
					"Could not load game data",
					"The game data is either missing or corrupted. Reinstall and try again",
					NULL
				);
				this->state.setShuttingDown();
			}

		}

		while (this->state.isRunning())
		{
			this->profiler->getFrameTimer().setStart();
			this->profiler->getProcessTimer().setStart();
			this->input();
			this->update();
			this->output();
			this->benchmark();
			this->profiler->getProcessTimer().setEnd();
			this->sleep();
			this->profiler->getFrameTimer().setEnd();
			this->profiler->getRunTimer().setEnd();
			this->profiler->incrementFrameCount();
		}

		this->game->deinitialize();
		this->shutdown();
	} while (this->state.isRestarting());
}

bool Lilliputian::Engine::initialize()
{
	bool isInitializedOK = false;

	this->state.setInitializing();

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		SDL_Log(
			"SDL could not initialize because: %s",
			SDL_GetError);
		this->state.setShuttingDown();
	}
	else
	{
		this->os = new OS("", 640, 480, false, this->argv[0]);
		this->game = new Game(this->os, this->profiler);
		this->aiSystem = new AISystem();
		this->animationSystem = new AnimationSystem();
		this->audioSystem = new AudioSystem();
		this->hapticSystem = new HapticSystem();
		this->physicsSystem = new PhysicsSystem();
		this->renderingSystem = new RenderingSystem(this->os->getWindow());
		this->uiSystem = new UISystem();

		isInitializedOK = true;
	}

	return isInitializedOK;
}

void Lilliputian::Engine::input()
{
	this->profiler->getInputTimer().setStart();
	HumanInterfaceDevices& hid = this->os->getHid();

	hid.detectGameControllers();
	hid.pollInputEvents();
	if (hid.hasRequestedShutdown())
		this->state.setShuttingDown();

	this->profiler->getInputTimer().setEnd();
}

void Lilliputian::Engine::update()
{
	this->profiler->getUpdateTimer().setStart();
	SceneForest activeScene = this->game->getActiveScene();
	const uint32_t msPerComputeUpdate = this->game->getConfiguration().msPerComputeUpdate;

	this->game->executeOnInputMethods();
	this->game->executeOnStartMethods();
	this->game->executeOnFrameMethods();
	this->uiSystem->process(activeScene, this->os->getHid());
	this->aiSystem->process(activeScene);

	while (this->profiler->getLag_ms() >= msPerComputeUpdate)
	{
		this->game->executeOnComputeMethods();
		this->animationSystem->process(activeScene);
		this->physicsSystem->process(activeScene);
		this->profiler->decrementLagCount(msPerComputeUpdate);
	}

	this->game->executeOnLateMethods();
	this->game->executeOnFinalMethods();
	this->profiler->incrementLagCount(this->profiler->getFrameTimer().getDelta_ns() / NS_IN_MS);
	this->profiler->getUpdateTimer().setEnd();
}

void Lilliputian::Engine::output()
{
	this->profiler->getOutputTimer().setStart();

	this->renderingSystem->process(this->game->getActiveScene());
	this->audioSystem->process(this->game->getActiveScene());
	this->hapticSystem->process(this->game->getActiveScene(), this->os->getHid());

	this->profiler->getOutputTimer().setEnd();
}

void Lilliputian::Engine::benchmark()
{
#ifdef _DEBUG
	uint32_t FPSUpdateSeconds = 1;

	if (this->profiler->getBenchmarkTimer().getDelta_ns() / NS_IN_MS >= (FPSUpdateSeconds * MS_IN_S))
	{
		this->profiler->getBenchmarkTimer().setStart();
		String FPSString = std::to_string(this->profiler->getFPS());
		String frameUtilizationString =
			std::to_string((int)(((double)this->profiler->getProcessTimer().getDelta_ns() / (double)this->profiler->getFrameTimer().getDelta_ns()) * 100));
		this->os->getWindow().changeTitle((this->game->getConfiguration().gameTitle + " (DEBUG) ->" +
			" FPS: " + FPSString +
			", Frame Time Utilization: " + frameUtilizationString + "%").c_str()
		);
	}
#endif
	this->profiler->getBenchmarkTimer().setEnd();
}

void Lilliputian::Engine::sleep()
{
	float targetFrameTime_ms = 1000.0 / this->game->getConfiguration().targetFPS;
	float processTime_ms = this->profiler->getProcessTimer().getDelta_ns() / NS_IN_MS;
	this->os->getWindow().sleep(targetFrameTime_ms - processTime_ms);
}

void Lilliputian::Engine::shutdown()
{
	delete this->os;
	delete this->game;
	delete this->aiSystem;
	delete this->animationSystem;
	delete this->audioSystem;
	delete this->hapticSystem;
	delete this->physicsSystem;
	delete this->renderingSystem;
	delete this->uiSystem;
	delete this->profiler;
}