#include "olcPixelGameEngine.h"
#include <iostream>
#include <vector>  


struct DDA_Result
{
	vi2d hit_cell;
	float distance;
	int side;
};


class RayCastWorld : public olc::PixelGameEngine
{
public:
	const float player_movement_speed = 5.0f;
	const float player_rotation_speed = 5.0f;

	float fov = 3.1415 / 4; 

	vf2d player_pos;
	vf2d player_dir = { -1, 0 };
	vf2d plane = { 0, 0.66 };

	float player_angle = 0;

	//std::vector<std::vector<int>> field;
	//vi2d field_size = { 20, 20 };

	vi2d field_size = { 24, 24 };
	int field[24][24] =
	{
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	bool OnUserCreate() override
	{
		player_pos = field_size / 2;
		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		Render();
		UpdatePlayer(dt);
		return true;
	}

	void UpdatePlayer(float dt)
	{
		if (GetKey(olc::Key::W).bHeld) 
		{
			if (field[int(player_pos.x + player_movement_speed * dt)][int(player_pos.y)] == 0)
			{
				player_pos.x += player_movement_speed * dt;
			}
			if (field[int(player_pos.x)][int(player_pos.y + player_movement_speed * dt)] == 0)
			{
				player_pos.y += player_movement_speed * dt;
			}
		} 

		if (GetKey(olc::Key::S).bHeld)
		{
			if (field[int(player_pos.x - player_movement_speed * dt)][int(player_pos.y)] == 0)
			{
				player_pos.x -= player_movement_speed * dt;
			}
			if (field[int(player_pos.x)][int(player_pos.y - player_movement_speed * dt)] == 0)
			{
				player_pos.y -= player_movement_speed * dt;
			}
		}
		
		if (GetKey(olc::Key::Q).bHeld)
		{
			plane      = RotateVector(-player_rotation_speed * dt, plane);
			player_dir = RotateVector(-player_rotation_speed * dt, player_dir);
		}

		if (GetKey(olc::Key::E).bHeld)
		{
			plane	   = RotateVector(player_rotation_speed * dt, plane);
			player_dir = RotateVector(player_rotation_speed * dt, player_dir);
		}
	}

	void Render()
	{
		Clear(olc::BLACK);

		for (int i = 0; i < ScreenWidth(); i++)
		{
			float x = ((2 * i) / ScreenWidth()) - 1;
			auto ray_dir = player_dir + plane * x;

			DDA_Result result = RunDDA(ray_dir);
			float h = ScreenHeight()*2 / result.distance;

			Pixel p = GetColorOfCell(result.hit_cell, result.side);

			FillRect(i, ScreenHeight() / 2 - h / 2, 1, h, p);
			//DrawLine(0, ScreenHeight() / 2, ScreenWidth(), ScreenHeight() / 2, olc::RED);
		}
	}

	vf2d RotateVector(float angle, vf2d v)
	{
		vf2d u;
		u.x = v.x * cosf(angle) - v.y * sinf(angle);
		u.y = v.x * sinf(angle) + v.y * cosf(angle);
		return u;
	}

	Pixel GetColorOfCell(vi2d cell_pos, int side)
	{
		Pixel p;

		switch (field[cell_pos.x][cell_pos.y])
		{
			case 1:  p = RED;    break; //red
			case 2:  p = GREEN;  break; //green
			case 3:  p = BLUE;   break; //blue
			case 4:  p = WHITE;  break; //white
			default: p = YELLOW; break; //yellow
		}

		if (side == 1) p /= 2;

		return p;
	}

	DDA_Result RunDDA(olc::vf2d dir)
	{
		float distance = 0;
		float max_distance = 100;
		int side = 0;

		bool hit = false;

		olc::vf2d RayUnitDistance = { sqrt(1 + (dir.y / dir.x) * (dir.y / dir.x)), sqrt(1 + (dir.x / dir.y) * (dir.x / dir.y)) };
		olc::vf2d RayTravelDistance;
		olc::vi2d MapCheck(player_pos);
		olc::vf2d Step;

		if (dir.x < 0)
		{
			Step.x = -1;
			RayTravelDistance.x = (player_pos.x - float(MapCheck.x)) * RayUnitDistance.x;
		}
		else
		{
			Step.x = 1;
			RayTravelDistance.x = (float(MapCheck.x + 1) - player_pos.x) * RayUnitDistance.x;
		}

		if (dir.y < 0)
		{
			Step.y = -1;
			RayTravelDistance.y = (player_pos.y - float(MapCheck.y)) * RayUnitDistance.y;
		}
		else
		{
			Step.y = 1;
			RayTravelDistance.y = (float(MapCheck.y + 1) - player_pos.y) * RayUnitDistance.y;
		}

		while (distance < max_distance && !hit)
		{
			if (RayTravelDistance.x <= RayTravelDistance.y)
			{
				MapCheck.x += Step.x;
				distance = RayTravelDistance.x;
				RayTravelDistance.x += RayUnitDistance.x;
				side = 0;
			}
			else
			{
				MapCheck.y += Step.y;
				distance = RayTravelDistance.y;
				RayTravelDistance.y += RayUnitDistance.y;
				side = 1;
			}

			if (MapCheck.x < 0 || MapCheck.x > field_size.x - 1 || MapCheck.y < 0 || MapCheck.y > field_size.y - 1)
			{
				break;
			}

			if (field[MapCheck.x][MapCheck.y] != 0)
			{
				hit = true;
			}
		}

		DDA_Result result;

		// get rid of fisheye effect
		if (side == 0) { result.distance = (RayTravelDistance.x - RayUnitDistance.x); }
		else		   { result.distance = (RayTravelDistance.y - RayUnitDistance.y); }

		//result.distance = distance; 
		result.hit_cell = MapCheck;
		result.side = side;
		return result;
	}
};