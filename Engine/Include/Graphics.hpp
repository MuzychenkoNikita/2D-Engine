#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <queue>
#include <vector>
#include <iostream>

#include "stb_image.h"
#define STBRP_LARGE_RECTS
#include "stb_rect_pack.h"

namespace Engine::Graphics {
class Camera
{
public:
    Camera();
    Camera(const Camera& other);

    inline float GetAspectRatio() const { return mAspectRatio; }
    inline float GetHeigth() const { return mHeight; }
    inline float GetNear() const { return mNear; }
    inline float GetFar() const { return mFar; }
    inline void SetViewMatrix(const glm::mat4& matrix) { mViewMatrix = matrix; }

    void SetAspectRatio(float aspectRatio);
    void SetHeight(float height);
    void setNear(float near);
    void SetFar(float far);
    void SetOrtho(float height, float near = 0.f, float far = 100.f);
    void SetViewMatrix(const glm::vec3& pos, float rot);

    const glm::mat4& GetProjectionMatrix() const { return mProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return mViewMatrix; }

private:
    void RecalculateProjectionMatrix();

private:
    float mAspectRatio;
    float mHeight, mNear, mFar;

    glm::mat4 mProjectionMatrix;
    glm::mat4 mViewMatrix;
};

struct AtlasRect {
    float x = 0, y = 0, w = 10, h = 10;
};

class Shader {
public:
    Shader();
    
    void Init(GLenum shaderType, const char* shaderCode);
    unsigned int GetID();
    
    ~Shader();
    
private:
    void CheckCompileErrors();
    
private:
    unsigned int mShaderID;
};

class ShaderProgram {
public:
    ShaderProgram();
    
    void Init(std::vector<unsigned int> shaderIDs);
    unsigned int GetID();
    void Use();
    
    ~ShaderProgram();
    
private:
    void CheckCompileErrors();
    
private:
    unsigned int mProgramID;
};

class ScreenEffect {
public:
    void Init(const char* code);
    void Use(float iTime, glm::vec2 iResolution);
    
private:
    Shader vertexShader;
    Shader fragmentShader;
    ShaderProgram effectShader;
    GLuint VAO;
};

class TextureAtlas
{
public:
    TextureAtlas();
    
    std::array<float, 8>* AddTexture(const std::string& path);
    void GenTexture();
    
    ~TextureAtlas() {}
    
    GLuint GetTextureID() { return mTextureID; }
    glm::ivec2 GetTextureSize() { return mTextureSize; }
    
private:
    GLuint mTextureID;
    glm::ivec2 mTextureSize;
    std::vector<std::string> mPaths;
    std::queue<unsigned char*> mTexturesData;
    int mTextureCount;
    std::deque<std::array<float, 8>> mAtlasRectsCoords;
};

class Animation
{
public:
    Animation();
    Animation(TextureAtlas* Atlas);
    
    void AddAnimationFrame(std::string path);
    void SetFrameDuration(float frameDuration) { mFrameDuration = frameDuration; }
    void SetTextureAtlas(TextureAtlas* Atlas) { mAtlas = Atlas; }
    std::array<float, 8>* GetCurrentFrameTextureData();
    
    ~Animation() {}
private:
    std::vector<std::array<float, 8>*> mFramesTextureData;
    float mFrameDuration;
    float mTimer;
    int mCurrentFrame;
    TextureAtlas* mAtlas;
    
};
}
