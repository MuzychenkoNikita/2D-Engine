#pragma once
#include "Core.hpp"

namespace Engine::Core
{
GameObject::GameObject()
    :mVertices{ 0.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 1.0f, 0.0f }
    ,mTexture{&mTexture0}
    ,mIndices{ 0, 1, 2,
                  1, 2, 3 }
    ,mPosition{0.0f, 0.0f}
    ,mSize{1.0f, 1.0f}
    ,mBufferOffset{NULL}
    {
    }
void GameObject::SetVertices(std::array<float, 12> vertices)
{
    mVertices = vertices;
}
void GameObject::SetTextureCoords(std::array<float, 8>* textureCoords)
{
    mTexture = textureCoords;
}
void GameObject::BindTexture(Engine::Graphics::TextureAtlas* Atlas, const std::string& path) {
    SetTextureCoords(Atlas->AddTexture(path));
}
    void GameObject::SetPosition(glm::vec2 position)
    {
        mPosition = position;
        UpdateVertices();
        std::cout<<"\nPlayer position: ( " << mPosition.x << " ; " << mPosition.y << " )";
    }
    void GameObject::SetSize(glm::vec2 size) {
        mSize = size;
        UpdateVertices();
    }
    void GameObject::UpdateVertices() {
        // ( 0 ; 0 )
        mVertices[0 * 3 + 0] = mPosition.x;
        mVertices[0 * 3 + 1] = mPosition.y;
        // ( 1 ; 0 )
        mVertices[1 * 3 + 0] = mPosition.x + mSize.x;
        mVertices[1 * 3 + 1] = mPosition.y;
        // ( 0 ; 1 )
        mVertices[2 * 3 + 0] = mPosition.x;
        mVertices[2 * 3 + 1] = mPosition.y + mSize.y;
        // ( 1 ; 1 )
        mVertices[3 * 3 + 0] = mPosition.x + mSize.x;
        mVertices[3 * 3 + 1] = mPosition.y + mSize.y;
    }
void GameObject::PlayAnimation(Engine::Graphics::Animation* Animation) {
    mTexture = Animation->GetCurrentFrameTextureData();
}
void PlayerCharacterObject::MoveX(bool Direction) {
    if (Direction) {
        SetPosition(glm::vec2(mPosition.x + (mSpeed * Engine::Core::DeltaTime), mPosition.y));
    } else {
        SetPosition(glm::vec2(mPosition.x - (mSpeed * Engine::Core::DeltaTime), mPosition.y));
    }
}    void PlayerCharacterObject::MoveY(bool Direction) {
    if (Direction)
        SetPosition(glm::vec2(mPosition.x, mPosition.y + (mSpeed * Engine::Core::DeltaTime)));
    else
        SetPosition(glm::vec2(mPosition.x, mPosition.y - (mSpeed * Engine::Core::DeltaTime)));
}
PlayerCharacterObject::PlayerCharacterObject() {
    
}
}
