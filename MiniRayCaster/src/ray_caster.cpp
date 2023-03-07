#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>  

using namespace std;


class Sketch : public olc::PixelGameEngine
{
public:
	const int w = 20;
	const int h = 20;

	int grid_w;
	int grid_h;

	std::vector<vector<int>> field;


	bool OnUserCreate() override
	{
		field = std::vector<vector<int>>(w, vector<int>(h));
		grid_w = ScreenWidth() / w;
		grid_h = ScreenHeight() / h;
		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		Clear(olc::DARK_BLUE);

		DrawGrid();
		UpdateGrid();

		return true;
	}

	void UpdateGrid()
	{
		if (GetMouse(0).bPressed)
		{
			int i = ScreenWidth() / w;
			int j = ScreenHeight() / h;

			field[i][j] = 1;
		}
	}

	void DrawGrid()
	{
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{

			}
		}

		for (int i = 0; i < w+1; i++) // x-axis
		{
			DrawLine(i*grid_w, 0, i*grid_w, ScreenHeight(), olc::YELLOW); // vertical line
		}

		for (int j = 0; j < h+1; j++) // y-axis
		{
			DrawLine(0, j*grid_h, ScreenWidth(), j*grid_h, olc::YELLOW); // horizontal line
		}
	}

};


int main()
{
	Sketch sketch;
	if (sketch.Construct(201, 201, 4, 4))
		sketch.Start();

	return 0;
}