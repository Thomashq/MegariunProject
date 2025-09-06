#include "Window.h"
#include <iostream>

struct MouseOffset
{
    float xoffset;
    float yoffset;
} mouseOffset;

bool isRunning = false;
bool firstMouse = true;
bool mouseIsMoving = false;
float lastX = 0.0f;
float lastY = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

Window::Window(int width, int height, const char* title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->Width = width;
	this->Height = height;
	this->Title = title;
}

Window::~Window()
{
    glfwTerminate();
}

bool Window::Init()
{
    this->window = glfwCreateWindow(Width, Height, Title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, this->Width, this->Height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
    glfwSetCursorPosCallback(this->window, mouse_callback);

    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    isRunning = true;
    return true;
}

void Window::ProcessInput()