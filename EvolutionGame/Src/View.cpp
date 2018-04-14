#include "View.h"
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

View::~View()
{
	delete presenter;
	DeleteObject(hPen);
	ReleaseDC(hWnd, hDC);
}

void View::Clear() 
{
	HBRUSH brush = CreateSolidBrush(0);
	FillRect(hDC, &screen, brush);
	GetClientRect(hWnd, &screen);
}

void View::Run() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	bool isSettingField = false;
	bool isStart = false;
	bool isStop = true;
	while (true)
	{
		if (isStart && !isStop)
		{
			presenter->RunGame();
		}
		WaitForSingleObject(hStdin, INFINITE);
		DWORD NumEvents;
		INPUT_RECORD InRec;
		BOOL b = ReadConsoleInputW(hStdin, &InRec, 1, &NumEvents);
		POINT pos;
		if (NumEvents != 0 && InRec.EventType == MOUSE_EVENT && InRec.Event.MouseEvent.dwButtonState == RI_MOUSE_BUTTON_1_DOWN)
		{
			GetCursorPos(&pos);
			ScreenToClient(hWnd, &pos);
			if (!isSettingField)
			{
				presenter->SettingField(Vector2(pos.x, pos.y));
				isSettingField = true;
			}
			else
			{
				//check: game is not starting?
				if (!isStart)
				{
					//check click on field and draw cell
					if (!presenter->ClickOnField(Vector2(pos.x, pos.y)))
					{
						if (presenter->ClickOnButten1(Vector2(pos.x, pos.y)))
						{
							isStart = true;
							isStop = false;
						}
					}

					//check click on refresh
					if (presenter->ClickOnButten2(Vector2(pos.x, pos.y)))
					{
						isStart = false;
						isStop = true;
					}
				}
				else
				{
					//check click on start or stop
					if (presenter->ClickOnButten1(Vector2(pos.x, pos.y)))
					{
						isStop = !isStop;
					}

					//check click on refresh
					if (presenter->ClickOnButten2(Vector2(pos.x, pos.y)))
					{
						isStart = false;
						isStop = true;
					}
				}
			}
		}
	}
}

void View::DrawLine(const Vector2& point1, const Vector2& point2) {
	MoveToEx(hDC, (int)point1.x, (int)point1.y, NULL);
	LineTo(hDC,(int)point2.x, (int)point2.y);
}

void View::SetColor(color col) {
	
	COLORREF pen_color;
	switch (col)
	{
		case white:
			pen_color = RGB(255, 255, 255);
			break;
		case red:
			pen_color = RGB(255, 0, 0);
			break;
		case transparent:
			pen_color = RGB(0, 0, 0);
			break;
		case green:
			pen_color = RGB(0, 255, 0);
			break;
		case yellow:
			pen_color = RGB(255, 255, 0);
			break;
		case blue:
			pen_color = RGB(0, 255, 255);
			break;
		case brown:
			pen_color = RGB(200, 200, 150);
			break;
	}

	hPen = CreatePen(PS_SOLID, 1, pen_color);

	SelectObject(hDC, hPen);
}