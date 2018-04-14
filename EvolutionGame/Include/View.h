#ifndef __VIEW
#define __VIEW
#include "Presenter.h"
#include "IView.h"
#include <windows.h>

class View : public IView
{
private:
	Presenter* presenter;
	HWND hWnd;

	HDC hDC;
	HPEN hPen;
	RECT screen;

public:
	View();

	~View();

	//for presenter
	void DrawLine(const Vector2&, const Vector2&);

	//for presenter
	void SetColor(color);

	//main func
	void Run();

	//not using
	void Clear();
};

#endif // !__VIEW


