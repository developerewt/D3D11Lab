#pragma once

#include <Renderer/Renderer.h>

class RendererManager
{
public:
	RendererManager() = default;
	~RendererManager() = default;

	static Renderer* CreateRenderer(Window* pWindow)
	{
		if(mRenderer == nullptr)
			mRenderer = new Renderer(pWindow);
		
		return mRenderer;
	}

	static Renderer* GetRenderer()
	{
		return mRenderer;
	}
private:
	static inline Renderer* mRenderer = nullptr;
};