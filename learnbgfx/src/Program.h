#ifndef LEARNBGFX_PROGRAM_H_
#define LEARNBGFX_PROGRAM_H_

#include <cstdlib>
#include <fstream>
#include <vector>

#include <bgfx/bgfx.h>

namespace learnbgfx {
    inline bgfx::ShaderHandle LoadShader(const char* filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        std::size_t size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<char> data(size);
        file.read(data.data(), size);
        const bgfx::Memory* memory = bgfx::copy(data.data(), data.size());
        memory->data[memory->size - 1] = '\0';
        return bgfx::createShader(memory);
    }

    inline bgfx::ProgramHandle LoadProgram(const char* vs, const char* fs) {
        bgfx::ShaderHandle vsh = LoadShader(vs);
        bgfx::ShaderHandle fsh = LoadShader(fs);
        return bgfx::createProgram(vsh, fsh, true);
    }
}


#endif