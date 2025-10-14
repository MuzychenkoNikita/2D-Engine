#include "Core.hpp"

#include "Levels.h"
#include "Objects.h"

int main()
{
    Engine::Core::Game MyGame(&Level_0_Atlas, &Level_0);
    Init_Level_0();
    
    MyGame.Render();
    
	return 0;
}
