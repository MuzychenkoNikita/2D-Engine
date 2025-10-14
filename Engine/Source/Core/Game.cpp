#include "Core.hpp"

namespace Engine::Core
{
Game::Game(Engine::Graphics::TextureAtlas* CurrentAtlas, Scene* CurrentScene)
    : GameDeltaTime(0.0)
    , GameWindowResolution({1920, 1080})
    , GameWindowTitle("Game")
    , GameCurrentAtlas(CurrentAtlas)
    , GameCurrentScene(CurrentScene)
    , GameCameraPos({1, 1, 1})
    , GameCameraZoom(1.0)
    , GameCameraSpeed(1.0)
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
    GameWindow = glfwCreateWindow(GameWindowResolution.x, GameWindowResolution.y, GameWindowTitle, NULL, NULL);
    if (GameWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(GameWindow);
    //glfwSetFramebufferSizeCallback(GameWindow, FramebufferSizeCallback);
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    
    // inializing interface
    InitGameInterface();

    InitShaders();
    
    projLoc = glGetUniformLocation(GameShader.GetID(), "proj");
    viewLoc = glGetUniformLocation(GameShader.GetID(), "view");
    
    GameShader.Use();
    GLint cur = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur);
    std::cout << "Active program after InitShaders: " << cur << "\n";
}
void Game::Render() {
    while (!GetWindowShouldClose()) {
        CalculateDeltaTime();
        CalculateFrameRate();
        ProcessInput();
        
        int fbw, fbh;
        glfwGetFramebufferSize(GameWindow, &fbw, &fbh);
        glViewport(0,0,fbw,fbh);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.f,0.f,0.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // camera + uniforms first
        GameCamera.SetHeight(GameCameraZoom);
        GameCamera.SetViewMatrix(GameCameraPos, 0.f);
        auto projection = GameCamera.GetProjectionMatrix();
        auto view       = GameCamera.GetViewMatrix();

        GameShader.Use();
        glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GameCurrentAtlas->GetTextureID());

        // scene
        GameCurrentScene->Render();
        
        // post effects
        if (ShowSnowShader) {
            SnowShader.Use(glfwGetTime(), GameWindowResolution);
        }
        
        // UI last
        GameInterface.Render();
        GameInterface.Clean();

        glfwSwapBuffers(GameWindow);
        glfwPollEvents();
    }
}
// Setters
void Game::SetCurrentAtlas(Engine::Graphics::TextureAtlas* CurrentAtlas) {
    GameCurrentAtlas = CurrentAtlas;
}
void Game::SetCurrentScene(Scene* CurrentScene) {
    GameCurrentScene = CurrentScene;
}
void Game::DefineGlobalGameObject() {
    
}
// Helpers
void GameFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void Game::ProcessInput() {
    static bool prev[GLFW_KEY_LAST + 1] = {false};
        auto pressed_once = [&](int key) {
            bool down  = glfwGetKey(GameWindow, key) == GLFW_PRESS;
            bool once  = down && !prev[key];   // edge: RELEASE -> PRESS
            prev[key]  = down;
            return once;
        };
    
    if (glfwGetKey(GameWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(GameWindow, true);
    
    if (glfwGetKey(GameWindow, GLFW_KEY_2) == GLFW_PRESS)
        GameCameraZoom = GameCameraZoom - 0.1f;
    if (glfwGetKey(GameWindow, GLFW_KEY_3) == GLFW_PRESS)
        GameCameraZoom = GameCameraZoom + 0.1f;
    

    if (glfwGetKey(GameWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
        GameCameraPos.x = GameCameraPos.x - (0.1f * GameCameraSpeed);
    if (glfwGetKey(GameWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
        GameCameraPos.x = GameCameraPos.x + (0.1f * GameCameraSpeed);
    if (glfwGetKey(GameWindow, GLFW_KEY_UP) == GLFW_PRESS)
        GameCameraPos.y = GameCameraPos.y + (0.1f * GameCameraSpeed);
    if (glfwGetKey(GameWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
        GameCameraPos.y = GameCameraPos.y - (0.1f * GameCameraSpeed);

    if (pressed_once(GLFW_KEY_GRAVE_ACCENT)) {
        GameInterface.ShowConsoleWindow(!GameInterface.GetConsoleP_open());
    }
}
void Game::CalculateDeltaTime() {
    if (!GameTimeFreeze) {
        static float LastTime = glfwGetTime();
        float CurrentTime = glfwGetTime();
        DeltaTime = CurrentTime - LastTime;
        LastTime = CurrentTime;
    }
}
void Game::CalculateFrameRate() {
    static int frames = 0;
    static double last = glfwGetTime();
    static int cachedFps = 0;

    ++frames;

    double now = glfwGetTime();
    double elapsed = now - last;

    if (elapsed >= 1.0) {                // update once per second
        cachedFps = int(frames / elapsed + 0.5);
        frames = 0;
        last = now;
    }

    GameFPS = cachedFps;
}
bool Game::GetWindowShouldClose() {
    return glfwWindowShouldClose(GameWindow);
}
Game::~Game() {
    ImGui_ImplOpenGL3_Shutdown();
    glfwTerminate();
}
void Game::InitGameInterface() {
    GameInterface.Init(GameWindow);
    GameInterface.SetTextureAtlas(GameCurrentAtlas);
    GameInterface.SetConsole(&GameConsole);
    GameInterface.SetFPS(&GameFPS);
    
    GameConsole.AddCommand("show_fps", [&](const char*) {
        GameInterface.ShowFPSWindow();
    });
    
    GameConsole.AddCommand("show_atlas", [&](const char*) {
        GameInterface.ShowAtlasWindow();
    });
    
    GameConsole.AddCommand("show_snow", [&](const char*) {
        if (ShowSnowShader) { ShowSnowShader = false; GameConsole.AddLog("[message] Let it snow!"); }
        else { ShowSnowShader = true; GameConsole.AddLog("[message] Let it snow!"); }
    });
}
}
