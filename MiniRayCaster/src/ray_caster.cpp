#define OLC_PGE_APPLICATION
#include "MapEditor.h"
#include "RayCastWorld.h"


int main()
{
	MapEditor editor;
	if (editor.Construct(201, 201, 4, 4))
		editor.Start();

	return 0;
}