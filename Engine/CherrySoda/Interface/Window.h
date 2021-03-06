#ifndef _CHERRYSODA_INTERFACE_WINDOW_H_
#define _CHERRYSODA_INTERFACE_WINDOW_H_

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/String.h>

#define CHERRYSODA_WINDOW SDL_Window
struct CHERRYSODA_WINDOW;

namespace cherrysoda {

class Window
{
private:
	friend class Engine;

	Window();

	void SetSize(int width, int height);
	void SetTitle(const String& string);
	void SetFullscreen(bool fullscreen);
	bool IsFullscreen();
	void ToggleFullscreen();

	void GetPosition(int* x, int* y);
	void SetMousePosition(int x, int y);

	void CreateWindow();
	void DestroyWindow();

	void Show();

	void PollEvents();

	static bool Initialize();
	static void Terminate();

	CHERRYSODA_WINDOW* m_mainWindow = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INTERFACE_WINDOW_H_
