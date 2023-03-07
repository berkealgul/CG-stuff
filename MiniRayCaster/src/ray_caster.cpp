#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>  

using namespace std;


class Sketch : public olc::PixelGameEngine
{
public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		return true;
	}
};


int main()
{
	Sketch sketch;
	if (sketch.Construct(101, 101, 4, 4))
		sketch.Start();

	return 0;
}