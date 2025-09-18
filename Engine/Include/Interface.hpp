#pragma once

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Graphics.hpp"

namespace Engine::Interface
{
// --++==[][] FUNCTIONS [][]==++--
// Interface Core
void InitiateImGui(GLFWwindow* window);
void RenderImGui();
void CleanRenderImGui();
void DestroyImGui();
void SetTextureAtlas(Engine::Graphics::TextureAtlas* AnAtlas);

// Interface window calls
void FrameRateWindow(); // Shows the rendered window Frame Rate
void ShowTextureAtlas(); // Shows chosen Texture Atlass Data for packing algorithm debug

// --++==[][] VARIABLES [][]==++--
extern GLFWwindow* mWindow;
extern Engine::Graphics::TextureAtlas* Atlas;
//Framebuffer& mCustomFramebuffer;
//Shader& mMainShader;
//float mFontMultiplier;
extern ImGuiIO* io;
//uint32_t addImportTextureID;
//GLuint specTex;
}
