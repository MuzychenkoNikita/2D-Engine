#include "Graphics.hpp"
#include "Core.hpp"

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#include <algorithm>
#include <deque>
#include <iostream>

namespace Engine::Graphics {
TextureAtlas::TextureAtlas()
{}

std::array<float, 8>* TextureAtlas::AddTexture(const std::string& path) {
    mPaths.push_back(path);
    mTextureCount++;
    mAtlasRectsCoords.emplace_back();
    return &mAtlasRectsCoords.back();
}

void TextureAtlas::GenTexture() {
    if (mPaths.empty()) return;

    stbi_set_flip_vertically_on_load(true);

    const int candidates[] = {128, 256, 512, 1024, 2048};

    for (int s = 0; s < (int)(sizeof(candidates)/sizeof(candidates[0])); ++s) {
        const int atlasW = candidates[s];
        const int atlasH = candidates[s];

        std::vector<stbrp_rect> rects(mPaths.size());
        std::vector<unsigned char*> pixels(mPaths.size(), nullptr);

        for (size_t i = 0; i < mPaths.size(); i++) {
            rects[i].id = (int)i;
            int w = 0, h = 0;
            unsigned char* data = stbi_load(mPaths[i].c_str(), &w, &h, nullptr, STBI_rgb_alpha);
            if (!data) {
                fprintf(stderr, "stbi_load failed: %s (%s)\n",
                        stbi_failure_reason(), mPaths[i].c_str());
                rects[i].w = rects[i].h = 0;
            } else {
                rects[i].w = w;
                rects[i].h = h;
            }
            pixels[i] = data;
        }

        stbrp_context ctx;
        std::vector<stbrp_node> nodes(atlasW);
        stbrp_init_target(&ctx, atlasW, atlasH, nodes.data(), atlasW);
        stbrp_pack_rects(&ctx, rects.data(), (int)rects.size());

        bool allPacked = std::all_of(rects.begin(), rects.end(),
                                     [](const stbrp_rect& r){ return r.was_packed != 0; });
        
        if (!allPacked) {
            for (auto* p : pixels) if (p) stbi_image_free(p);
            continue;
        }
        
        int RectID = 0;
        for (auto& CurrentTextureRect : mAtlasRectsCoords) {
            float x = rects[RectID].x;
            float y = rects[RectID].y;
            float w = rects[RectID].w;
            float h = rects[RectID].h;
            float size = atlasW;
            CurrentTextureRect = {
                x / size, y / size,
                (x + w) / size, y / size,
                x / size, (y + h) / size,
                (x + w) / size, (y + h) / size
            };
            RectID++;
        }

        glGenTextures(1, &mTextureID);
        glBindTexture(GL_TEXTURE_2D, mTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glPixelStorei(GL_UNPACK_ALIGNMENT,   1);
        glPixelStorei(GL_UNPACK_ROW_LENGTH,  0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, atlasW, atlasH, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        for (size_t i = 0; i < rects.size(); ++i) {
            if (!pixels[i]) continue;
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                            rects[i].x, rects[i].y,
                            rects[i].w, rects[i].h,
                            GL_RGBA, GL_UNSIGNED_BYTE,
                            pixels[i]);
            stbi_image_free(pixels[i]);
        }

        mTextureSize = {atlasW, atlasH};
        return;
    }

    fprintf(stderr, "TextureAtlas: images did not fit into max candidate size.\n");
}
Animation::Animation()
: mFrameDuration(0.f), mTimer(0.f), mCurrentFrame(0), mAtlas(nullptr) {}
Animation::Animation(TextureAtlas* Atlas)
: mFrameDuration(0.f), mTimer(0.f), mCurrentFrame(0), mAtlas(Atlas) {}

void Animation::AddAnimationFrame(std::string path) {
    if (!mAtlas) return;
    std::array<float,8>* uv = mAtlas->AddTexture(path);
    if (uv) mFramesTextureData.push_back(uv);
}
std::array<float, 8>* Animation::GetCurrentFrameTextureData() {
    if (mFramesTextureData.empty()) return nullptr;

    if (mFrameDuration > 0.f) {
        mTimer += Engine::Core::DeltaTime;
        while (mTimer >= mFrameDuration) {
            mTimer -= mFrameDuration;
            mCurrentFrame = (mCurrentFrame + 1) % mFramesTextureData.size();
        }
    }

    return mFramesTextureData[mCurrentFrame];
}
}
