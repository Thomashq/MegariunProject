#pragma once
#include "Globals.h"
#include "Window.h"

enum class AppState
{
	Gameloop,
	Menu,
	Exit
};

class App
{
public:
	Window *window;
	AppState state;

	App(Window* window);
	~App();

	void Init();
	void Run();
	void Stop();

private:
	void Render();

	Canvas* canvas;
};