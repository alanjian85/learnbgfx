#include <cmath>
#include <iostream>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "Program.h"
#include "Texture.h"
using namespace learnbgfx;

static bool quit = false;
static float mix_value = 0.2f;

void windowResizeCallback(SDL_Window* window, int width, int height) {
    bgfx::reset(width, height);
    bgfx::setViewRect(0, 0, 0, width, height);
}

void processInput() {
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_ESCAPE]) {
        quit = true;
    }

    if (state[SDL_SCANCODE_UP]) {
        mix_value += 0.001f;
        if (mix_value >= 1.0f)
            mix_value = 1.0f;
    }
    
    if (state[SDL_SCANCODE_DOWN]) {
        mix_value -= 0.001f;
        if (mix_value <= 0.0f)
            mix_value = 0.0f;
    }
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
        // positions         // colors          // texture coords
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f
    };

    std::uint16_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
    .end();
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), layout);
    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));

    bgfx::ProgramHandle program = loadProgram("vs_shader.bin", "fs_shader.bin");

    bgfx::UniformHandle s_texture1 = bgfx::createUniform("s_texture1", bgfx::UniformType::Sampler);
    bgfx::UniformHandle s_texture2 = bgfx::createUniform("s_texture2", bgfx::UniformType::Sampler);
    bgfx::UniformHandle u_params = bgfx::createUniform("u_params", bgfx::UniformType::Vec4);

    bgfx::TextureHandle texture1 = loadTexture("container.dds");
    bgfx::TextureHandle texture2 = loadTexture("awesomeface.dds");

    bgfx::setViewRect(0, 0, 0, 800, 600);

    bgfx::setTexture(0, s_texture1, texture1);
    bgfx::setTexture(1, s_texture2, texture2);

    float view[16];
    bx::mtxLookAt(view, bx::Vec3(0.0f, 0.0f, 3.0f), bx::Vec3(0.0f, 0.0f, 0.0f));

    float proj[16];
    bx::mtxProj(proj, 
        45.0f, 
        800.0f / 600.0f,
        0.1f, 100.0f,
        bgfx::getCaps()->homogeneousDepth
    );

    bgfx::setViewTransform(0, view, proj);

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
            }
        }

        processInput();

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0x334d4d);
        bgfx::touch(0);

        float transform[16];
        bx::mtxSRT(transform, 
            1.0f, 1.0f, 1.0f,
            bx::toRad(55.0f), 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f
        );
        bgfx::setTransform(transform);

        float params[] = { mix_value, 0.0f, 0.0f, 0.0f };
        bgfx::setUniform(u_params, params);

        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);
        bgfx::setState(BGFX_STATE_WRITE_RGB);
        bgfx::submit(0, program);

        bgfx::frame();
    }

    bgfx::shutdown();
    SDL_Quit();
    return 0;
}