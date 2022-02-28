#ifndef LEARNBGFX_TEXTURE_H_
#define LEARNBGFX_TEXTURE_H_

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <vector>

#include <bgfx/bgfx.h>
#include <bimg/decode.h>
#include <bx/allocator.h>

namespace learnbgfx {
    bgfx::TextureHandle loadTexture(const char* filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        std::size_t size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<char> data(size);
        file.read(data.data(), size);
        bx::DefaultAllocator allocator;
        bimg::ImageContainer* image = bimg::imageParse(&allocator, data.data(), data.size());
        return bgfx::createTexture2D(
            image->m_width,
            image->m_height,
            1 < image->m_numMips,
            image->m_numLayers, 
            bgfx::TextureFormat::Enum(image->m_format),
            BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE,
            bgfx::makeRef(image->m_data, image->m_size)
        );
    }
}

#endif