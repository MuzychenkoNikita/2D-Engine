#include "Levels.h"
#include "Core.hpp"
#include "Objects.h"
#include "Graphics.hpp"


Engine::Core::Scene Level_0;  // Actual definition

void Init_Level_0() {
    Level_0.SetNumberOfStaticObjects(420);
    
    // Static Objects
    auto* tile = new Engine::Core::GameObject();
    tile->SetPosition(glm::vec2(1, 0));
    Level_0.AddStaticObject(tile);

    auto* tile2 = new Engine::Core::GameObject();
    tile2->SetPosition(glm::vec2(0, 0));
    Level_0.AddStaticObject(tile2);
    
    for (int i = 0; i < 100; i++) {
        auto* tiles = new Engine::Core::GameObject();
        tiles->SetPosition(glm::vec2(i - 50, 10));
        Level_0.AddStaticObject(tiles);
    }
    for (int i = 0; i < 100; i++) {
        auto* tiles = new Engine::Core::GameObject();
        tiles->SetPosition(glm::vec2(i - 50, -90));
        Level_0.AddStaticObject(tiles);
    }
    for (int i = 0; i < 100; i++) {
        auto* tiles = new Engine::Core::GameObject();
        tiles->SetPosition(glm::vec2(-50, i - 90));
        Level_0.AddStaticObject(tiles);
    }
    for (int i = 0; i <= 100; i++) {
        auto* tiles = new Engine::Core::GameObject();
        tiles->SetPosition(glm::vec2(50, i -90));
        Level_0.AddStaticObject(tiles);
    }

    // Dynamic Objects
    Player.SetPosition(glm::vec2(-2, 0));
    Level_0.AddDynamicObject(&Player);

    Level_0.InitGL();                  
    Level_0.UploadAllStaticToBuffer();
    Level_0.UploadAllDynamicToBuffer();
}
