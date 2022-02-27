#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

void windowResizeCallback(SDL_Window* window, int width, int height) {
    bgfx::reset(width, height);
    bgfx::setViewRect(0, 0, 0, width, height);
}

bgfx::ShaderHandle loadShader(const char* filename) {
    std::ifstream file(filename, std::ios::ate);
    std::size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> data(size);
    file.read(data.data(), size);
    const bgfx::Memory* memory = bgfx::copy(data.data(), data.size());
    memory->data[memory->size - 1] = '\0';
    return bgfx::createShader(memory);
}

int main() {
    SDL_Init(0);
    SDL_Window* window = SDL_CreateWindow(
        "LearnBgfx", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (window == nullptr) {
        std::cerr << "Failed to create SDL2 window" << std::endl;
        SDL_Quit();
        return -1;
    }

    bgfx::renderFrame();

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    SDL_GetWindowWMInfo(window, &wmi);

    bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = wmi.info.x11.display;
    init.platformData.nwh = reinterpret_cast<void*>(wmi.info.x11.window);
#elif BX_PLATFORM_OSX
    init.platformData.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = wmi.info.win.window;
#endif
    init.resolution.width = 800;
    init.resolution.height = 600;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.type = bgfx::RendererType::OpenGL;
    if (!bgfx::init(init)) {
        std::cerr << "Failed to initialize BGFX" << std::endl;
        return -1;
    }

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .end();
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), layout);

    bgfx::ShaderHandle vsh = loadShader("vs_shader.bin");
    bgfx::ShaderHandle fsh = loadShader("fs_shader.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh);
    bgfx::destroy(vsh);
    bgfx::destroy(fsh);

    bgfx::setViewRect(0, 0, 0, 800, 600);

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            windowResizeCallback(window, event.window.data1, event.window.data2);
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                    }
            }

            bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x334d4d);
            bgfx::touch(0);

            bgfx::setVertexBuffer(0, vbh);
            bgfx::submit(0, program);

            bgfx::frame();
        }
    }

    bgfx::shutdown();
    SDL_Quit();
    return 0;
}