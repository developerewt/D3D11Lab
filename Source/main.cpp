#include <Window/Window.h>
#include <Renderer/RendererManager.h>

#include <stdio.h>
#include <vector>

#include "Object/Object.h"

std::vector<VertexData> vertices =
{
	{ XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{ XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
};

std::vector<VertexData> vertices2 =
{
	{ XMFLOAT3(0.5f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
	{ XMFLOAT3(0.75f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
};

std::vector<uint32_t> indices = { 0, 1, 2 };

int main()
{
	Window* basicWindow = new Window({ 1280, 720 }, "D3D11Lab");
	basicWindow->Initialize();

	Renderer* basicRenderer = RendererManager::CreateRenderer(basicWindow);
	Object* triangle = new Object(vertices, indices);
	Object* triangle2 = new Object(vertices2, indices);
	std::vector<Object*> objects;
	objects.push_back(triangle);
	objects.push_back(triangle2);

	while(basicWindow->IsVisible())
	{
		basicWindow->Run();
		basicRenderer->ClearColor({ 0.2f, 0.3f, 0.4f });
		basicRenderer->SetPipeline();
		
		for(auto& obj : objects)
		{
			obj->SetProps();
			basicRenderer->Draw(obj->GetIndexCount());
		}
		
		basicRenderer->Present();
	}

	delete basicWindow;
	delete basicRenderer;
	return 0;
}
