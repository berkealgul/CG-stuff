#define OLC_PGE_APPLICATION
#include "MapEditor.h"
#include "RayCastWorld.h"


int main()
{
	//MapEditor editor;
	//if (editor.Construct(201, 201, 4, 4))
	//	editor.Start();

	RayCastWorld editor;
	if (editor.Construct(640, 480, 1, 1))
		editor.Start();

	return 0;
}