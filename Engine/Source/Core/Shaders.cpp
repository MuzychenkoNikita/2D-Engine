#include "Core.hpp"

namespace Engine::Core
{
void Game::InitShaders() {
    GameVertexShader.Init(GL_VERTEX_SHADER,
    R"(
    #version 330 core

    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    uniform mat4 proj;
    uniform mat4 view;

    out vec2 TexCoord;

    void main() {
        gl_Position = proj * view * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        TexCoord = aTexCoord;
    }
    )");
        
    GameFragmentShader.Init(GL_FRAGMENT_SHADER,
    R"(
    #version 330 core

    uniform mat4 proj;
    uniform mat4 view;
    uniform sampler2D ourTexture;

    out vec4 FragColor;
    in vec2 TexCoord;

    void main()
    {
        vec4 texColor = texture(ourTexture, TexCoord);
        FragColor = vec4(texColor);
    }
    )");
        
    GameShader.Init({ GameVertexShader.GetID(), GameFragmentShader.GetID() });
        
    SnowShader.Init(
    R"(
    #version 330 core
    in vec2 vUV;
    uniform float iTime;
    out vec4 FragColor;

    float snow(vec2 uv,float scale){
        float w = smoothstep(1.,0.,-uv.y*(scale/10.));
        if(w < .1) return 0.;
        uv += iTime/scale;
        uv.y += iTime*2./scale;
        uv.x += sin(uv.y + iTime*.5)/scale;

        uv *= scale;
        vec2 s = floor(uv), f = fract(uv), p;
        float k = 3., d;

        p = .5 + .35 * sin(11. * fract(sin((s + scale) * mat2(7,3,6,5)) * 5.)) - f;

        d = length(p);
        k = min(d, k);
        k = smoothstep(0., k, sin(f.x + f.y) * 0.01);
        return k * w;
    }

    void main() {
        // Accumulate flake coverage (mask)
        float a = 0.0;
        a += snow(vUV,30.)*.3;
        a += snow(vUV,20.)*.5;
        a += snow(vUV,15.)*.8;
        a += snow(vUV,10.);
        a += snow(vUV, 8.);
        a += snow(vUV, 6.);
        a += snow(vUV, 5.);
        a = clamp(a, 0.0, 1.0);          // final alpha

        vec3 snowColor = vec3(1.0);       // white flakes
        FragColor = vec4(snowColor, a);   // background stays alpha=0
    }

    )");
}
}
