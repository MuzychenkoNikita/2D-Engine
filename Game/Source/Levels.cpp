#include "Levels.h"
#include "Engine/Core.h"
#include "Objects.h"


Core::Scene Level_0;  // Actual definition

void Init_Level_0() {
    //Static
    auto* tile = new Core::GameObject();
    std::array<float, 12> a{ {1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f } };
    tile->SetVertices(a);
    Level_0.AddStaticObject(tile);

    auto* tile2 = new Core::GameObject();
    std::array<float, 12> b{ {1.0f + 1.f, 1.0f + 1.f, 0.0f,
                              0.0f + 1.f, 1.0f + 1.f, 0.0f,
                              0.0f + 1.f, 0.0f + 1.f, 0.0f,
                              1.0f + 1.f, 0.0f + 1.f, 0.0f } };
    tile2->SetVertices(b);
    Level_0.AddStaticObject(tile2);

    //Dynamic
    std::array<float, 12> c{ {-1.0f, 1.0f, 0.0f,
                              -0.0f, 1.0f, 0.0f,
                              -0.0f, 0.0f, 0.0f,
                              -1.0f, 0.0f, 0.0f } };
    Player.SetVertices(c);
    Level_0.AddDynamicObject(&Player);

    Level_0.InitGL();                  
    Level_0.UploadAllStaticToBuffer();
    Level_0.UploadAllDynamicToBuffer();
}
