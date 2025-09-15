#include "Graphics.hpp"
#include "stb_rect_pack.h"
#include "stb_image.h"

#include <queue>
#include <iostream>

namespace Engine::Graphics {
TextureAtlas::TextureAtlas()
{}
void TextureAtlas::AddTexture(std::string path) {
    
}
void TextureAtlas::GenTexture() {
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
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
    
}

Animation::Animation(TextureAtlas& Atlas)
{}
void Animation::AddAnimationFrame(std::string path) {
    mAnimationFrames.push(path);
}
}
