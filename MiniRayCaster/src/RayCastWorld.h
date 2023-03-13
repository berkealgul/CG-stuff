#include "olcPixelGameEngine.h"
#include <vector>  

class RayCastWorld : public olc::PixelGameEngine
{
public:

	int screen_w_ = 201;
	int screen_h_ = 201;

	const int w = 20;
	const int h = 20;

	int grid_w;
	int grid_h;

	int last_edited_cell_i = -1;
	int last_edited_cell_j = -1;

	std::vector<std::vector<int>> field;

	bool OnUserCreate() override
	{
		grid_w = ScreenWidth() / w;
		grid_h = ScreenHeight() / h;
		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		//Clear(olc::DARK_BLUE);

		return true;
	}

	void Render()
	{
		for (int i = 0; i < screen_w_; i++)
		{

		}
	}
};