#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <glad/glad.h>

namespace Core {

	class GameObject
	{

	public:
		GameObject();

		void SetVertices(std::array<float, 12> vertices);
		void SetTextureCoords(std::array<float, 8> textureCoords);
		void SetPosition(glm::vec2 position);
		void SetBufferOffset(size_t bufferOffset) { mBufferOffset = bufferOffset; }
		//void SetTexture();

		virtual ~GameObject() {}

		const std::array<float, 12>& GetVertices() const { return mvertices; }
		const std::array<float, 8>& GetTextureCoords() const { return mtextureCoords; }
		const std::array<int, 6>& GetIndices() const { return mIndices; }
		glm::vec2 GetPosition() const { return mposition; }
		size_t GetBufferOffset() const { return mBufferOffset; }

	private:
		std::array<float, 12> mvertices;
		std::array<float, 8> mtextureCoords;
		std::array<int, 6> mIndices;
		glm::vec2 mposition;
		size_t mBufferOffset;

	};


	class PlayerCharacterObject : public GameObject
	{

	public:
		PlayerCharacterObject();


		~PlayerCharacterObject() {}

	private:
		float mVelocity;

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

	class TextureAtlas
	{

	public:
		TextureAtlas();

		~TextureAtlas() {}
	private:

	};
}