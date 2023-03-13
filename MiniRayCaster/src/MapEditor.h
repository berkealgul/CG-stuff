#include "olcPixelGameEngine.h"
#include <vector>  

class MapEditor : public olc::PixelGameEngine
{
public:
	const int w = 20;
	const int h = 20;

	int grid_w;
	int grid_h;

	int last_edited_cell_i = -1;
	int last_edited_cell_j = -1;

	std::vector<std::vector<int>> field;


	bool OnUserCreate() override
	{
		field = std::vector<std::vector<int>>(w, std::vector<int>(h));
		grid_w = ScreenWidth() / w;
		grid_h = ScreenHeight() / h;
		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		//Clear(olc::DARK_BLUE);

		DrawGrid();
		UpdateGrid();

		olc::vf2d pos = { (float)w / 2, (float)h / 2 };
		olc::vf2d dir = (GetMousePos() - pos).norm();

		auto checkpoints = RunDDA(pos, dir);

		//std::vector<olc::vf2d>checkpoints;
		//checkpoints.push_back(pos);
		//checkpoints.push_back(GetMousePos());

		DrawDDARay(checkpoints);

		return true;
	}

	void DrawDDARay(std::vector<olc::vf2d> checkpoints)
	{
		DrawLine(checkpoints[0], checkpoints[checkpoints.size() - 1]);
		for (auto v : checkpoints)
		{
			FillCircle(v, 1);
		}
	}

	void UpdateGrid()
	{
		if (GetMouse(0).bHeld)
		{
			int i = GetMouseX() / grid_w;
			int j = GetMouseY() / grid_h;

			if (i == last_edited_cell_i && j == last_edited_cell_j) { return; }

			field[i][j] = field[i][j] == 1 ? 0 : 1;
			last_edited_cell_i = i;
			last_edited_cell_j = j;
		}
	}

	void DrawGrid()
	{
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				olc::Pixel p;

				if (field[i][j] == 0)
				{
					p = olc::DARK_BLUE;
				}
				else
				{
					p = olc::DARK_RED;
				}
				FillRect(i * grid_w, j * grid_h, grid_w, grid_h, p);
				DrawRect(i * grid_w, j * grid_h, grid_w, grid_h, olc::YELLOW);
			}
		}
	}

	std::vector<olc::vf2d> RunDDA(olc::vf2d pos, olc::vf2d dir)
	{
		float distance = 0;
		float max_distance = 100;

		bool hit = false;

		olc::vf2d RayUnitDistance = { sqrt(1 + dir.x / dir.y), sqrt(1 + dir.y / dir.x) };
		olc::vf2d RayTravelDistance;
		olc::vi2d MapCheck(pos);
		olc::vf2d step;

		std::vector<olc::vf2d> Checkpoints;

		Checkpoints.push_back(olc::vf2d(MapCheck));

		if (dir.x < 0)
		{
			step.x = -1;
		}
		else
		{
			step.x = 1;
		}

		if (dir.y < 0)
		{
			step.y = -1;
		}
		else
		{
			step.y = 1;
		}

		Checkpoints.push_back(olc::vf2d(MapCheck));

		while (distance < max_distance && !hit)
		{
			if (RayTravelDistance.x < RayTravelDistance.y)
			{
				MapCheck.x += step.x;
				distance = RayTravelDistance.x;
				RayTravelDistance.x += RayUnitDistance.x;
			}
			else
			{
				MapCheck.y += step.y;
				distance = RayTravelDistance.y;
				RayTravelDistance.y += RayUnitDistance.y;
			}

			Checkpoints.push_back(olc::vf2d(MapCheck));

			if (field[MapCheck.x][MapCheck.y] != 0)
			{
				hit = true;
			}
		}

		return Checkpoints;
	}

};