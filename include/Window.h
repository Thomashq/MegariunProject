#include "Globals.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	int Width, Height;
	const char* Title = nullptr;
	Window(int width, int height, const char* title);
	~Window();

	bool Init();

	void ProcessInput(Camera& camera);

	void Update();

	bool WindowIsRunning();

	double GetTime();

	void Stop();
private:
	GLFWwindow *window = nullptr;

};