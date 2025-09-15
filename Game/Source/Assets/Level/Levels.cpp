#include "Levels.h"
#include "Core.hpp"
#include "Objects.h"


Engine::Core::Scene Level_0;  // Actual definition

void Init_Level_0() {
    // Static Objects
    auto* tile = new Engine::Core::GameObject();
    tile->SetPosition(glm::vec2(1, 0));
    Level_0.AddStaticObject(tile);

    auto* tile2 = new Engine::Core::GameObject();
    tile2->SetPosition(glm::vec2(0, 0));
    Level_0.AddStaticObject(tile2);

    // Dynamic Objects
    Player.SetPosition(glm::vec2(-2, 0));
    Level_0.AddDynamicObject(&Player);

    Level_0.InitGL();                  
    Level_0.UploadAllStaticToBuffer();
    Level_0.UploadAllDynamicToBuffer();
}
