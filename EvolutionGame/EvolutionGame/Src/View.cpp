#include "..\Include\View.h"
View::View()
{
	presenter = new Presenter(this);
	hWnd = GetConsoleWindow();
	hDC = GetDC(hWnd);
	hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	SelectObject(hDC, hPen);
	GetClientRect(hWnd, &screen);

	DWORD Mode;
	GetConsoleMode(hWnd, &Mode);
	SetConsoleMode(hWnd, Mode | ENABLE_MOUSE_INPUT);
}

void View::Initialization()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	bool isClickForField = false;
	while (true) 
	{
		WaitForSingleObject(hStdin, INFINITE);
		DWORD NumEvents;
		BOOL b = ReadConsoleInputW(hStdin, &InRec, 3, &NumEvents);
		POINT pos;
		if (NumEvents != 0 && InRec.EventType == MOUSE_EVENT && InRec.Event.MouseEvent.dwButtonState == RI_MOUSE_BUTTON_1_DOWN) 
		{
			GetCursorPos(&pos);
			ScreenToClient(hWnd, &pos);
			if (!isClickForField)
			{
				presenter->SettingField(Vector2(pos.x, pos.y));
				isClickForField = true;
			}
			else if (!presenter->ClickOnField(Vector2(pos.x, pos.y)))
			{
				//hStdin = GetStdHandle(STD_OUTPUT_HANDLE);
				break;
			}
		}
	}
	
	return;
}

void View::Clear() 
{
	HBRUSH brush = CreateSolidBrush(0);
	FillRect(hDC, &screen, brush);
}

void View::Run() {
	Initialization();
	presenter->RunGame();
}

void View::DrawLine(const Vector2& point1, const Vector2& point2) {
	MoveToEx(hDC, (int)point1.x, (int)point1.y, NULL);
	LineTo(hDC,(int)point2.x, (int)point2.y);
}

void View::SetColor(color col) {
	
	COLORREF pen_color;
	if (col == red) {
		pen_color = RGB(255, 0, 0);
	}
	if (col == white) {
		pen_color = RGB(255, 255, 255);
	}
	if (col == transparent) {
		pen_color = RGB(0, 0, 0);
	}

	hPen = CreatePen(PS_SOLID, 1, pen_color);

	SelectObject(hDC, hPen);
}