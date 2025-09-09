#include "Core.h"

#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <glad/glad.h>

namespace Core {
	GameObject::GameObject()
		:mvertices{ 1.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f }
		,mtextureCoords{ 0.0f, 0.0f,
						1.0f, 0.0f,
						1.0f, 1.0f,
						0.0f, 1.0f }
		,mIndices{ 0, 1, 3,
				  1, 2, 3 }
		,mposition{0.0f, 0.0f}
		,mBufferOffset{NULL}
	{
	}
	void GameObject::SetVertices(std::array<float, 12> vertices)
	{
		mvertices = vertices;
	}
	void GameObject::SetTextureCoords(std::array<float, 8> textureCoords)
	{
		mtextureCoords = textureCoords;
	}
	void GameObject::SetPosition(glm::vec2 position)
	{
		mposition = position;
		for (int i = 0; i < 4; i++) {
			mvertices[i * 3 + 0] += mposition.x;
			mvertices[i * 3 + 1] += mposition.y;
		}
	}


	Scene::Scene()
		:mNumberOfStaticObjects{10}
		,mNumberOfDynamicObjects{10}
	{
		
	}
	void Scene::InitGL()
	{
		//Static Buffers
		glGenVertexArrays(1, &mStaticVAO);
		glGenBuffers(1, &mStaticVBO);
		glGenBuffers(1, &mStaticEBO);

		glBindVertexArray(mStaticVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mStaticVBO);
		glBufferData(GL_ARRAY_BUFFER, mNumberOfStaticObjects * 20 * sizeof(float), nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mStaticEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumberOfStaticObjects * 6 * sizeof(int), nullptr, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //texCoords
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		//Dynamic Buffers
		glGenVertexArrays(1, &mDynamicVAO);
		glGenBuffers(1, &mDynamicVBO);
		glGenBuffers(1, &mDynamicEBO);

		glBindVertexArray(mDynamicVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mDynamicVBO);
		glBufferData(GL_ARRAY_BUFFER, mNumberOfDynamicObjects * 20 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mDynamicEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumberOfDynamicObjects * 6 * sizeof(int), nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //texCoords
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}
	void Scene::AddStaticObject(GameObject* object)
	{
		mStaticObjects.push_back(object);
	}
	void Scene::AddDynamicObject(GameObject* object)
	{
		object->SetBufferOffset(mNextBufferOffset);
		mDynamicObjects.push_back(object);
		mNextBufferOffset += 20 * sizeof(float);
	}
	void Scene::UploadAllStaticToBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mStaticVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mStaticEBO);
		std::vector<float> mVBOData;
		std::vector<int> mEBOData;

		int vertexOffset = 0;
		for (auto* object : mStaticObjects) {
			const auto& mVertices = object->GetVertices();
			const auto& mTextureCoords = object->GetTextureCoords();
			const auto& mIndices = object->GetIndices();

			for (int i = 0; i < 4; i++) {
				mVBOData.push_back(mVertices[i * 3 + 0]); //x
				mVBOData.push_back(mVertices[i * 3 + 1]); //y
				mVBOData.push_back(mVertices[i * 3 + 2]); //z
				mVBOData.push_back(mTextureCoords[i * 2 + 0]); //u
				mVBOData.push_back(mTextureCoords[i * 2 + 1]); //v
				std::cout << "VBO Succes" << std::endl;
			}
			for (int i = 0; i < 6; i++) {
				mEBOData.push_back(mIndices[i] + vertexOffset);
			}
			vertexOffset += 4;
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, mVBOData.size() * sizeof(float), mVBOData.data());
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mEBOData.size() * sizeof(int), mEBOData.data());
	}
	void Scene::UploadAllDynamicToBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mDynamicVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mDynamicEBO);
		std::vector<float> mVBOData;
		std::vector<int> mEBOData;

		int vertexOffset = 0;
		for (auto* object : mDynamicObjects) {
			const auto& mVertices = object->GetVertices();
			const auto& mTextureCoords = object->GetTextureCoords();
			const auto& mIndices = object->GetIndices();

			for (int i = 0; i < 4; i++) {
				mVBOData.push_back(mVertices[i * 3 + 0]); //x
				mVBOData.push_back(mVertices[i * 3 + 1]); //y
				mVBOData.push_back(mVertices[i * 3 + 2]); //z
				mVBOData.push_back(mTextureCoords[i * 2 + 0]); //u
				mVBOData.push_back(mTextureCoords[i * 2 + 1]); //v
			}
			for (int i = 0; i < 6; i++) {
				mEBOData.push_back(mIndices[i] + vertexOffset);
			}
			vertexOffset += 4;
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, mVBOData.size() * sizeof(float), mVBOData.data());
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mEBOData.size() * sizeof(int), mEBOData.data());
	}
	void Scene::UpdateDynamicObject(GameObject* object)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mDynamicVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mDynamicEBO);
		std::vector<float> mVBOData;
		const auto& mVertices = object->GetVertices();
		const auto& mTextureCoords = object->GetTextureCoords();

		for (int i = 0; i < 4; i++) {
			mVBOData.push_back(mVertices[i * 3 + 0]); //x
			mVBOData.push_back(mVertices[i * 3 + 1]); //y
			mVBOData.push_back(mVertices[i * 3 + 2]); //z
			mVBOData.push_back(mTextureCoords[i * 2 + 0]); //u
			mVBOData.push_back(mTextureCoords[i * 2 + 1]); //v
		}
		glBufferSubData(GL_ARRAY_BUFFER, object->GetBufferOffset(), mVBOData.size() * sizeof(float), mVBOData.data());
	}
	void Scene::Render()
	{
		//Draw Static Objects
		glBindVertexArray(mStaticVAO);
		glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

		//Draw Dynamic Objects
		glBindVertexArray(mDynamicVAO);
		glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

		//std::cout << "Render Succes" << std::endl;
	}
	Scene::~Scene()
	{
		glDeleteBuffers(1, &mStaticVBO);
		glDeleteBuffers(1, &mStaticEBO);
		glDeleteVertexArrays(1, &mStaticVAO);

		glDeleteBuffers(1, &mDynamicVBO);
		glDeleteBuffers(1, &mDynamicEBO);
		glDeleteVertexArrays(1, &mDynamicVAO);
	}
	PlayerCharacterObject::PlayerCharacterObject()
	{
	}
}