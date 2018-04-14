#ifndef __PRESENTOR
#define __PRESENTOR
#include "IView.h"

class Presenter
{
private:
	IView * view;
	int scale = 30;
	int width;
	int height;
	int ButtenIndexX;
	int ButtenIndexY;
	bool** field;

	//In start
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

		//draw butten
		view->DrawLine(Vector2(ButtenIndexX * scale, ButtenIndexY * scale), Vector2((ButtenIndexX + 1) * scale, ButtenIndexY * scale));
		view->DrawLine(Vector2(ButtenIndexX * scale, ButtenIndexY * scale), Vector2(ButtenIndexX * scale, (ButtenIndexY + 1) * scale));
		view->DrawLine(Vector2((ButtenIndexX + 1) * scale, (ButtenIndexY + 1) * scale), Vector2((ButtenIndexX + 1) * scale, ButtenIndexY * scale));
		view->DrawLine(Vector2((ButtenIndexX + 1) * scale, (ButtenIndexY + 1) * scale), Vector2(ButtenIndexX * scale, (ButtenIndexY + 1) * scale));

	}
	
	void clearCell(int indexX, int indexY)
	{
		view->SetColor(transparent);
		view->DrawLine(Vector2(indexX * scale, indexY * scale), Vector2((indexX + 1) * scale, (indexY + 1) * scale));
		view->DrawLine(Vector2((indexX + 1) * scale, indexY * scale), Vector2(indexX* scale, (indexY + 1) * scale));
		view->SetColor(white);
	}
	
	void drawCell(int indexX, int indexY)
	{
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
		//pause
		for (size_t i = 0; i < 10000000; i++){}
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int count = countNeighbors(j, i);
				if (field[i][j] && (count == 0 || count == 1 || count > 3))
				{
					field[i][j] = false;
					clearCell(j, i);
				}
				if (!field[i][j] && count == 3)
				{
					field[i][j] = true;
					drawCell(j, i);
				}
			}
		}
	}
public:
	Presenter(IView* view)
	{
		this->view = view;
	}

	//when Initialization size field
	void SettingField(Vector2 size)
	{
		this->width = int(size.x) / scale;
		this->height = int(size.y) / scale;
		field = new bool*[height];
		for (size_t i = 0; i < height; i++)
		{
			field[i] = new bool[width];
			for (size_t j = 0; j < width; j++)
			{
				field[i][j] = false;
			}
		}
		ButtenIndexX = width + 1;
		ButtenIndexY = 0;
		drawField();
	}

	//when Initialization starting cells and check finish 
	bool ClickOnField(Vector2 posClick)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (posClick.x > j * scale &&  posClick.x < (j + 1) * scale &&
					posClick.y > i * scale &&  posClick.y < (i + 1) * scale
					)
				{
					if (!field[i][j])
					{
						field[i][j] = true;
						drawCell(j, i);
						return true;
					}
				}
			}
		}

		if (posClick.x > ButtenIndexX * scale &&  posClick.x < (ButtenIndexX + 1) * scale &&
			posClick.y > ButtenIndexY * scale &&  posClick.y < (ButtenIndexY + 1) * scale
			)
		{
			return false;
		}
		return true;
	}

	//start game
	void RunGame()
	{
		while (true)
		{
			drawScene();
		}
	}
};

#endif // !__PRESENTOR

