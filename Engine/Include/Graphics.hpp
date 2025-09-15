#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <queue>

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

class TextureAtlas
{
public:
    TextureAtlas();
    
    void AddTexture(std::string path);
    void GenTexture();

    ~TextureAtlas() {}
    
    GLuint GetTexture() { return mTextureID; }
    
private:
    GLuint mTextureID;

};

class Animation
{
public:
    Animation(TextureAtlas& Atlas);
    
    void AddAnimationFrame(std::string path);
    void SetFrameDuration();
    
    ~Animation() {}
private:
    std::queue<std::string> mAnimationFrames;
    float mFrameDuration;
    
};
}
