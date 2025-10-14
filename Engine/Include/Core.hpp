#pragma once

#include <iostream>
#include <array>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include "Graphics.hpp"
#include "Interface.hpp"

namespace Engine::Core 
{
class GameObject {
public:
    GameObject();

    void SetVertices(std::array<float, 12> vertices);
    void SetTextureCoords(std::array<float, 8>* textureCoords);
    void BindTexture(Engine::Graphics::TextureAtlas* AnAtlas, const std::string& path);
    void SetPosition(glm::vec2 position);
    void SetSize(glm::vec2 size);
    void SetBufferOffset(size_t bufferOffset) { mBufferOffset = bufferOffset; }
    void PlayAnimation(Engine::Graphics::Animation* Animation);
    //void SetTexture();

    virtual ~GameObject() {}

    const std::array<float, 12>& GetVertices() const { return mVertices; }
    std::array<float, 8>* GetTextureCoords() const { return mTexture; }
    const std::array<int, 6>& GetIndices() const { return mIndices; }
    glm::vec2 GetPosition() const { return mPosition; }
    size_t GetBufferOffset() const { return mBufferOffset; }

protected:
    void UpdateVertices();
    
protected:
    std::array<float, 12> mVertices;
    std::array<float, 8>* mTexture;
    std::array<float,8> mTexture0 = {0, 0, 0, 0, 0, 0, 0, 0};
    std::array<int, 6> mIndices;
    glm::vec2 mPosition;
    glm::vec2 mSize;
    size_t mBufferOffset;

};


class PlayerCharacterObject : public GameObject {

public:
    PlayerCharacterObject();
    
    void SetSpeed(float speed) { mSpeed = speed; };
    void MoveX(bool Direction);
    void MoveY(bool Direction);
    
    ~PlayerCharacterObject() {}
    
    float GetSpeed() { return mSpeed; }

private:
    float mSpeed;

};


class NonPlayerCharacterObject : public GameObject {

public:
    NonPlayerCharacterObject();

    ~NonPlayerCharacterObject() {}

private:

};


class TileObject : public GameObject {

public:
    TileObject();

    ~TileObject() {}

private:

};


class Scene {
public:
    Scene();

    void InitGL();
    void AddStaticObject(GameObject* object);
    void AddDynamicObject(GameObject* object);
    void UploadAllStaticToBuffer();
    void UploadAllDynamicToBuffer();
    void UpdateDynamicObject(GameObject* object);
    void Render();

    void SetNumberOfStaticObjects(int numberOfStaticObjects) { mNumberOfStaticObjects = numberOfStaticObjects; }
    void SetNumberOfDynamicObjects(int numberOfDynamicObjects) { mNumberOfDynamicObjects = numberOfDynamicObjects; }

    ~Scene();

private:
    std::vector<GameObject*> mStaticObjects;
    std::vector<GameObject*> mDynamicObjects;
    GLuint mStaticVAO, mDynamicVAO;
    GLuint mStaticVBO, mDynamicVBO;
    GLuint mStaticEBO, mDynamicEBO;

    int mNumberOfStaticObjects;
    int mNumberOfDynamicObjects;
    size_t mNextBufferOffset = 0;
};
    

extern float DeltaTime;

class Game {
public:
    Game(Engine::Graphics::TextureAtlas* CurrentAtlas, Scene* CurrentScene);
    
    void Render();
    
    void DefineGlobalGameObject();
    void AddInputAction(void (*fn_args)(const char*));
    
    // Setters
    void SetCurrentAtlas(Engine::Graphics::TextureAtlas* CurrentAtlas);
    void SetCurrentScene(Scene* CurrentScene);
    
    // Helper Functions
    void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void ProcessInput();
    void CalculateDeltaTime();
    void CalculateFrameRate();
    
    // Getters
    GLFWwindow* GetWindow() { return GameWindow; }
    bool GetWindowShouldClose();
    
    ~Game();
    
private:
    void InitGameInterface();
    
private:
    Engine::Interface::Interface GameInterface;
    Engine::Interface::Console GameConsole;
    Engine::Graphics::TextureAtlas GameGlobalAtlas;
    Engine::Graphics::TextureAtlas* GameCurrentAtlas;
    Scene* GameCurrentScene;
    
    // initilazing camera
    Engine::Graphics::Camera GameCamera;
    glm::vec3 GameCameraPos;
    float GameCameraSpeed;
    float GameCameraZoom;
    
    // initializing shaders
    void InitShaders();
    Engine::Graphics::Shader GameVertexShader;
    Engine::Graphics::Shader GameFragmentShader;
    Engine::Graphics::ShaderProgram GameShader;
    
    bool ShowSnowShader;
    Engine::Graphics::ScreenEffect SnowShader;
    GLuint dummyVAO = 0;
    
    int projLoc;
    int viewLoc;
    
    GLFWwindow* GameWindow;
    glm::vec2 GameWindowResolution;
    char* GameWindowTitle;
    
    float GameDeltaTime;
    bool GameTimeFreeze;
    int GameFPS;
};
}
