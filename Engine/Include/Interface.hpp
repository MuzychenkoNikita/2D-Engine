#pragma once

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Engine::Interface
{
// --++==[][] FUNCTIONS [][]==++--
void InitiateImGUI(GLFWwindow* window);
void RenderImGUI();
void CleanImGUI();
void FrameRateWindow();

// --++==[][] VARIABLES [][]==++--
extern GLFWwindow* mWindow;
//Framebuffer& mCustomFramebuffer;
//Shader& mMainShader;
//float mFontMultiplier;
extern ImGuiIO* io;
//uint32_t addImportTextureID;
//GLuint specTex;
}
