#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include <iostream>

#include "Interface.hpp"
#include "Graphics.hpp"
#include "Core.hpp"

#include "Levels.h"
#include "Objects.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void CheckBorder(float vertices[], int XBorder, int YBorder);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
constexpr float GamePixelSize = 0.01;

float XOffset = 0.0f;
float YOffset = 0.0f;
float Velocity = 1.0f;

float AnimationOffset = 0.f;
float AnimationChange = 23.f;
int AnimationCount = 0;
float LastTime = 0.f;

float Zoom = 1.f;
float CamXOffset = 0.f;
float CamYOffset = 0.f;

float XBorder = 1.f;
float YBorder = 1.f;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"uniform mat4 proj;\n"
"uniform mat4 view;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"   gl_Position = proj * view * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   TexCoord = aTexCoord;\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"uniform mat4 proj;\n"
"uniform mat4 view;\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"   vec4 texColor = texture(ourTexture, TexCoord);\n"
"   FragColor = vec4(texColor);\n"
"}\n\0";

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Game", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
    Engine::Interface::InitiateImGui(window);
	// initilazing camera
    Engine::Graphics::Camera mCamera;
	glm::vec3 mCameraPos;

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	int projLoc = glGetUniformLocation(shaderProgram, "proj");
	int viewLoc = glGetUniformLocation(shaderProgram, "view");
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



    
    
    


    
    
	Init_Level_0();
    
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// render
		// ------
        Engine::Interface::RenderImGui();
        
		glClearColor(0.2f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        

		glUseProgram(shaderProgram);

		// binding texture
        glBindTexture(GL_TEXTURE_2D, Level_0_Atlas.GetTextureID());
        
        // render scene
		Level_0.Render();

		mCamera.SetHeight(Zoom);
		glm::mat4 projection = mCamera.GetProjectionMatrix();
		mCamera.SetViewMatrix(glm::vec3(CamXOffset, CamYOffset, 0), 0.f);
		glm::mat4 view = mCamera.GetViewMatrix();
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        Engine::Interface::CleanRenderImGui();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteProgram(shaderProgram);
	
    Engine::Interface::DestroyImGui();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Player.MoveX(0);
        Player.PlayAnimation(&PlayerAnimationLeft);
        Level_0.UpdateDynamicObject(&Player);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Player.MoveX(1);
        Player.PlayAnimation(&PlayerAnimationRight);
        Level_0.UpdateDynamicObject(&Player);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Player.MoveY(1);
        Player.PlayAnimation(&PlayerAnimationBack);
        Level_0.UpdateDynamicObject(&Player);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Player.MoveY(0);
        Player.PlayAnimation(&PlayerAnimationFront);
        Level_0.UpdateDynamicObject(&Player);
    }
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        Player.SetSpeed(Player.GetSpeed() - 0.1);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        Player.SetSpeed(Player.GetSpeed() + 0.1);

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        Player.SetPosition(glm::vec2(0, 0));
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		Zoom = Zoom - 0.1f;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		Zoom = Zoom + 0.1f;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		CamXOffset = CamXOffset - (0.1f * Velocity);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		CamXOffset = CamXOffset + (0.1f * Velocity);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		CamYOffset = CamYOffset + (0.1f * Velocity);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		CamYOffset = CamYOffset - (0.1f * Velocity);

}
					
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width `and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void CheckBorder(float vertices[], int XBorder, int YBorder) {
	for (int i = 0; i < sizeof(vertices); i = i + 3) {
		if (vertices[i] > XBorder) {
			XOffset = XBorder - (0.1f);
		}
		if (vertices[i] < -XBorder) {
			XOffset = -XBorder + (0.1f);
		}
	}
	for (int i = 1; i < sizeof(vertices); i = i + 3) {
		if (vertices[i] > YBorder) {
			YOffset = YBorder - 0.1;
		}
		if (vertices[i] < -YBorder) {
			YOffset = -YBorder + 0.1;
		}
	}
}

