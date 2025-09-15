#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
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
"layout (location = 2) in vec2 aTexCoord;\n"
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
    Engine::Interface::InitiateImGUI(window);
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



	// generating texture bruh 8====0
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Source/texture/player_character.png", &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	std::cout << width << height << std::endl;
	stbi_image_free(data);



	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3,  // second Triangle
	};
	/*
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	*/


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Init_Level_0();
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
        
		// Printing Frame Rate
        if (Engine::Core::GetFrameRate() != 0)
            std::cout<<"\nFrame Rate: "<<Engine::Core::GetFrameRate()<<" FPS";
		// input
		// -----
		processInput(window);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// render
		// ------
        Engine::Interface::RenderImGUI();
        
		glClearColor(0.2f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        
        Level_0.UpdateDynamicObject(&Player);

		glUseProgram(shaderProgram);

		// binding texture
		glBindTexture(GL_TEXTURE_2D, texture);
		Level_0.Render();
		/*
		glBindVertexArray(VAO);

		

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		*/

		mCamera.SetHeight(Zoom);
		glm::mat4 projection = mCamera.GetProjectionMatrix();
		mCamera.SetViewMatrix(glm::vec3(CamXOffset, CamYOffset, 0), 0.f);
		glm::mat4 view = mCamera.GetViewMatrix();
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        Engine::Interface::CleanImGUI();


		//glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);
			
		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	/*
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);*/
	glDeleteProgram(shaderProgram);
	
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Player.MoveX(0);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Player.MoveX(1);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Player.MoveY(1);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Player.MoveY(0);
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

