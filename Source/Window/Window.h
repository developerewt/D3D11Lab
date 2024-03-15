#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <string>

struct WindowSize
{
	int X, Y;
};

class Window
{
public:
	/// @brief 
	/// @param size 
	/// @param title 
	Window(WindowSize size, const std::string& title);

	/// @brief
	~Window();

	/// @brief
	void Initialize();

	/// @brief
	void Run();

	/// @brief
	bool IsVisible() const { return !glfwWindowShouldClose(mWindow); };
	/// @brief
	GLFWwindow* GetWindowHandler() const { return mWindow; }
	/// @brief
	HWND GetWindowNativeHandler() const { return glfwGetWin32Window(mWindow); }
	/// @brief
	WindowSize GetSize() const { return mSize; }
private:
	GLFWwindow* mWindow;
	WindowSize mSize;
	std::string mTitle;
};