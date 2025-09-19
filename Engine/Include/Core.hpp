#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <glad/glad.h>

#include "Graphics.hpp"

namespace Engine::Core {

	class GameObject
	{

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


	class PlayerCharacterObject : public GameObject
	{

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


	class NonPlayerCharacterObject : public GameObject
	{

	public:
		NonPlayerCharacterObject();

		~NonPlayerCharacterObject() {}

	private:

	};


	class TileObject : public GameObject
	{

	public:
		TileObject();

		~TileObject() {}

	private:

	};


	class Scene 
	{

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
    
    // --== Functions ==--
    void CalculateDeltaTime();
    int GetFrameRate();
    // --== Variables ==--
    extern float DeltaTime;
}
