#include "Graphics.hpp"

namespace Engine::Graphics
{
Shader::Shader() {
    mShaderID = 0;
}
void Shader::Init(GLenum shaderType, const char* shaderCode) {
    mShaderID = glCreateShader(shaderType);
    
    glShaderSource(mShaderID, 1, &shaderCode, NULL);
    glCompileShader(mShaderID);
    
    CheckCompileErrors();
}
unsigned int Shader::GetID() {
    return mShaderID;
}
Shader::~Shader() {
    if (mShaderID) glDeleteShader(mShaderID);
}
void Shader::CheckCompileErrors() {
    int success;
    char infoLog[1024];
    glGetShaderiv(mShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(mShaderID, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}
}

namespace Engine::Graphics
{
ShaderProgram::ShaderProgram() {
    mProgramID = 0;
}
void ShaderProgram::Init(std::vector<unsigned int> shaderIDs) {
    mProgramID = glCreateProgram();
    for (auto shaderID : shaderIDs) glAttachShader(mProgramID, shaderID);
    glLinkProgram(mProgramID);
    CheckCompileErrors();
}
unsigned int ShaderProgram::GetID() {
    return mProgramID;
}
void ShaderProgram::Use() {
    glUseProgram(mProgramID);
}
ShaderProgram::~ShaderProgram() {
    if (mProgramID) glDeleteProgram(mProgramID);
}
void ShaderProgram::CheckCompileErrors() {
    int success;
    char infoLog[1024];
    glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(mProgramID, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}
void ScreenEffect::Init(const char* code) {
    vertexShader.Init(GL_VERTEX_SHADER,
    R"(
    #version 330 core
    out vec2 vUV;
    void main() {
        const vec2 pos[3] = vec2[3](vec2(-1,-1), vec2(3,-1), vec2(-1,3));
        vec2 p = pos[gl_VertexID];
        gl_Position = vec4(p, 0.0, 1.0);
        vUV = 0.5 * (p + 1.0); // 0..1
    }
    )");
    
    fragmentShader.Init(GL_FRAGMENT_SHADER, code);
    effectShader.Init({ vertexShader.GetID(), fragmentShader.GetID() });
    glGenVertexArrays(1, &VAO);
}
void ScreenEffect::Use(float iTime, glm::vec2 iResolution) {
    effectShader.Use();                      // bind program first
    glBindVertexArray(VAO);

    glUniform1f(glGetUniformLocation(effectShader.GetID(), "iTime"), iTime);
    glUniform2f(glGetUniformLocation(effectShader.GetID(), "iResolution"), iResolution.x, iResolution.y);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
}
