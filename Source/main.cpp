#include <Window/Window.h>

int main()
{
	Window* basicWindow = new Window({ 1280, 720 }, "D3D11Lab");

	basicWindow->Initialize();

	while(basicWindow->IsVisible())
	{
		basicWindow->Run();
	}

	delete basicWindow;
	return 0;
}
