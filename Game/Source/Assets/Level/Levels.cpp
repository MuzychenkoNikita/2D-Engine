#include "Levels.h"
#include "Core.hpp"
#include "Objects.h"
#include "Graphics.hpp"
#include "Interface.hpp"


Engine::Core::Scene Level_0;  // Actual definition
Engine::Graphics::TextureAtlas Level_0_Atlas;

Engine::Graphics::Animation PlayerAnimationFront(&Level_0_Atlas);
Engine::Graphics::Animation PlayerAnimationBack(&Level_0_Atlas);
Engine::Graphics::Animation PlayerAnimationRight(&Level_0_Atlas);
Engine::Graphics::Animation PlayerAnimationLeft(&Level_0_Atlas);

Engine::Core::GameObject Map;

void Init_Level_0() {
    Level_0.SetNumberOfStaticObjects(420);
    
              // real object
    Engine::Interface::SetTextureAtlas(&Level_0_Atlas);
    /*
    Level_0_Atlas.AddTexture("Assets/Texture/player_character.png");
    Level_0_Atlas.AddTexture("Assets/Texture/player_character.jpg");
    Level_0_Atlas.AddTexture("Assets/Texture/texture2.png");
    Level_0_Atlas.AddTexture("Assets/Texture/texture1.png");
    Level_0_Atlas.AddTexture("Assets/Texture/texture5.png");
    Level_0_Atlas.AddTexture("Assets/Texture/texture3.png");
    Level_0_Atlas.AddTexture("Assets/Texture/texture4.png");
    Level_0_Atlas.AddTexture("Assets/Texture/1player_character.png");
    //Player.BindTexture(&Level_0_Atlas, "Assets/Texture/Player_Character/idle_front.png");
     */
    Map.BindTexture(&Level_0_Atlas, "Assets/Texture/texture3.png");
    Map.SetSize({25, 25});
    Map.SetPosition({-10, -10});
    Level_0.AddStaticObject(&Map);
    
    
    PlayerAnimationFront.SetFrameDuration(0.2f);
    PlayerAnimationFront.AddAnimationFrame("Assets/Texture/Player_Character/idle_front.png");
    PlayerAnimationFront.AddAnimationFrame("Assets/Texture/Player_Character/idle_front_move1.png");
    PlayerAnimationFront.AddAnimationFrame("Assets/Texture/Player_Character/idle_front.png");
    PlayerAnimationFront.AddAnimationFrame("Assets/Texture/Player_Character/idle_front_move2.png");
    
   
    PlayerAnimationBack.SetFrameDuration(0.2f);
    PlayerAnimationBack.AddAnimationFrame("Assets/Texture/Player_Character/idle_back.png");
    PlayerAnimationBack.AddAnimationFrame("Assets/Texture/Player_Character/idle_back_move.png");
    
    
    PlayerAnimationRight.SetFrameDuration(0.2f);
    PlayerAnimationRight.AddAnimationFrame("Assets/Texture/Player_Character/idle_right.png");
    PlayerAnimationRight.AddAnimationFrame("Assets/Texture/Player_Character/idle_right_move.png");
    
    
    PlayerAnimationLeft.SetFrameDuration(0.2f);
    PlayerAnimationLeft.AddAnimationFrame("Assets/Texture/Player_Character/idle_left.png");
    PlayerAnimationLeft.AddAnimationFrame("Assets/Texture/Player_Character/idle_left_move.png");
    
    Level_0_Atlas.GenTexture();
    
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
