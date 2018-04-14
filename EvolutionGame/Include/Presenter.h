#ifndef __PRESENTOR
#define __PRESENTOR
#include "IView.h"

class Presenter
{
private:
	IView * view;
	int scale = 20;
	int width;
	int height;
	int Butten1IndexX;
	int Butten1IndexY;
	int Butten2IndexX;
	int Butten2IndexY;
	bool firstClickOnButten = true;
	long long sizePause = 10000000;
	bool** field;
	bool** fieldTemp;

	void pause()
	{
		for (long long i = 0; i < sizePause; i++) {}
	}

	//in start 
	void drawField()
	{
		//draw border
		view->DrawLine(Vector2(0, 0), Vector2(width * scale, 0));
		view->DrawLine(Vector2(0, 0), Vector2(0, height * scale));
		view->DrawLine(Vector2(width * scale, 0), Vector2(width * scale, height * scale));
		view->DrawLine(Vector2(0, height * scale), Vector2(width * scale, height * scale));

		//draw horizontal line
		for (int i = 0; i < height; i++)
		{
			view->DrawLine(Vector2(0, i * scale), Vector2(width * scale, i * scale));
		}

		//draw vertical line
		for (int j = 0; j < width; j++)
		{
			view->DrawLine(Vector2(j * scale, 0), Vector2(j * scale, height * scale));
		}

		//draw butten1
		view->DrawLine(Vector2(Butten1IndexX * scale, Butten1IndexY * scale), Vector2((Butten1IndexX + 1) * scale, Butten1IndexY * scale));
		view->DrawLine(Vector2(Butten1IndexX * scale, Butten1IndexY * scale), Vector2(Butten1IndexX * scale, (Butten1IndexY + 1) * scale));
		view->DrawLine(Vector2((Butten1IndexX + 1) * scale, (Butten1IndexY + 1) * scale), Vector2((Butten1IndexX + 1) * scale, Butten1IndexY * scale));
		view->DrawLine(Vector2((Butten1IndexX + 1) * scale, (Butten1IndexY + 1) * scale), Vector2(Butten1IndexX * scale, (Butten1IndexY + 1) * scale));

		//draw butten2
		view->DrawLine(Vector2(Butten2IndexX * scale, Butten2IndexY * scale), Vector2((Butten2IndexX + 1) * scale, Butten2IndexY * scale));
		view->DrawLine(Vector2(Butten2IndexX * scale, Butten2IndexY * scale), Vector2(Butten2IndexX * scale, (Butten2IndexY + 1) * scale));
		view->DrawLine(Vector2((Butten2IndexX + 1) * scale, (Butten2IndexY + 1) * scale), Vector2((Butten2IndexX + 1) * scale, Butten2IndexY * scale));
		view->DrawLine(Vector2((Butten2IndexX + 1) * scale, (Butten2IndexY + 1) * scale), Vector2(Butten2IndexX * scale, (Butten2IndexY + 1) * scale));

	}
	
	void clearCell(int indexX, int indexY)
	{
		drawCell(indexX, indexY, transparent);
	}
	
	void drawCell(int indexX, int indexY, color col)
	{
		view->SetColor(col);
		view->DrawLine(Vector2(indexX * scale, indexY * scale), Vector2((indexX + 1) * scale, (indexY + 1) * scale));
		view->DrawLine(Vector2((indexX + 1) * scale, indexY * scale), Vector2(indexX* scale, (indexY + 1) * scale));
	}
	
	//for drawScene
	int countNeighbors(int indexX, int indexY)
	{
		int result = 0;
		if ((indexY - 1) >= 0 && field[indexY - 1][indexX] == true)result++;
		if ((indexY - 1) >= 0 && (indexX - 1) >= 0 && field[indexY - 1][indexX - 1] == true)result++;
		if ((indexX - 1) >= 0 && field[indexY][indexX - 1] == true)result++;
		if ((indexY - 1) >= 0 && (indexX + 1) < width && field[indexY - 1][indexX + 1] == true)result++;
		if ((indexX + 1) < width && field[indexY][indexX + 1] == true)result++;
		if ((indexY + 1) < height && (indexX + 1) < width && field[indexY + 1][indexX + 1] == true)result++;
		if ((indexY + 1) < height && field[indexY + 1][indexX] == true)result++;
		if ((indexY + 1) < height && (indexX - 1) >= 0 && field[indexY + 1][indexX - 1] == true)result++;
		return result;
	}
	
	//check condition and draw cells
	void drawScene()
	{
		//new Info
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int count = countNeighbors(j, i);
				fieldTemp[i][j] = field[i][j];
				if (field[i][j] && (count == 0 || count == 1 || count > 3))
				{
					fieldTemp[i][j] = false;
				}
				if (!field[i][j] && count == 3)
				{
					fieldTemp[i][j] = true;
				}
			}
		}

		//set newInfo
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (field[i][j] != fieldTemp[i][j])
				{
					field[i][j] = fieldTemp[i][j];
					if (field[i][j])
					{
						drawCell(j, i, white);
					}
					if (!field[i][j])
					{
						clearCell(j, i);
					}
				}
			}
		}
	}

	//check click this area
	bool isClickInArea(int indexX, int indexY, Vector2 posClick)
	{
		if (posClick.x > indexX * scale &&  posClick.x < (indexX + 1) * scale &&
			posClick.y > indexY * scale &&  posClick.y < (indexY + 1) * scale
			)
		{
			return true;
		}
		return false;
	}

	//for refresh
	void clearField()
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (field[i][j])
				{
					field[i][j] = false;
					clearCell(j, i);
				}
			}
		}
	}
public:
	Presenter(IView* view)
	{
		this->view = view;
	}

	~Presenter()
	{
		delete view;
		for (int i = 0; i < height; i++)
		{
			delete[] fieldTemp[i];
			delete[] field[i];
		}
		delete[] fieldTemp;
		delete[] field;
	}

	//when Initialization size field
	void SettingField(Vector2 size)
	{
		this->width = int(size.x) / scale;
		this->height = int(size.y) / scale;
		field = new bool*[size_t(this->height)];
		fieldTemp = new bool*[size_t(this->height)];
		for (int i = 0; i < height; i++)
		{
			field[i] = new bool[size_t(this->width)];
			fieldTemp[i] = new bool[size_t(this->width)];
			for (int j = 0; j < width; j++)
			{
				field[i][j] = false;
			}
		}
		Butten1IndexX = width + 1;
		Butten1IndexY = 0;

		Butten2IndexX = width + 1;
		Butten2IndexY = 2;
		drawField();
	}

	//when Initialization starting cells
	bool ClickOnField(Vector2 posClick)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (isClickInArea(j,i, posClick))
				{
					if (!field[i][j])
					{
						field[i][j] = true;
						drawCell(j, i, white);
					}
					return true;
				}
			}
		}
		return false;
	}

	//start and stop game
	bool ClickOnButten1(Vector2 posClick)
	{
		if (isClickInArea(Butten1IndexX, Butten1IndexY, posClick))
		{
			if (firstClickOnButten)
			{
				drawCell(Butten1IndexX, Butten1IndexY, green);
				firstClickOnButten = false;
			}
			else
			{
				drawCell(Butten1IndexX, Butten1IndexY, red);
				firstClickOnButten = true;
			}
			return true;
		}
		return false;
	}

	//refresh scene
	bool ClickOnButten2(Vector2 posClick)
	{
		if (isClickInArea(Butten2IndexX, Butten2IndexY, posClick))
		{
			clearField();
			clearCell(Butten1IndexX, Butten1IndexY);
			return true;
		}
		return false;
	}

	//start game
	void RunGame()
	{
		pause();
		drawScene();
	}
};

#endif // !__PRESENTOR

