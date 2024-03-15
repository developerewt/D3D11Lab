#include <Window/Window.h>

Window::Window(WindowSize size, const std::string& title) : mSize(size), mTitle(title)
{ }

Window::~Window()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void Window::Initialize()
{
	if (!glfwInit())
		abort();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	mWindow = glfwCreateWindow(mSize.X, mSize.Y, mTitle.c_str(), nullptr, nullptr);
	if (!mWindow)
	{
		glfwTerminate();
		abort();
	}

	// HWND hwnd = glfwGetWin32Window(mWindow);
	// ShowWindow(hwnd, SW_SHOW);
}

void Window::Run()
{
	glfwPollEvents();
}