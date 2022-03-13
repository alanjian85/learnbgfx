#include <cmath>
#include <iostream>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "Camera.h"
#include "Program.h"
#include "Texture.h"
using namespace learnbgfx;

namespace {
    bool quit = false;
    float mix_value = 0.2f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    Camera camera;
}

void windowResizeCallback(SDL_Window* window, int width, int height) {
    bgfx::reset(width, height);
    bgfx::setViewRect(0, 0, 0, width, height);
}

void mouseMoveCallback(SDL_Window* window, Sint32 xrel, Sint32 yrel) {
    camera.processMouseMovement(xrel, -yrel);
}

void mouseScrollCallback(SDL_Window* window, Sint32 x, Sint32 y) {
    camera.processMouseScroll(y);
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
    
    if (state[SDL_SCANCODE_W])
        camera.processKeyboard(Orientation::kFront, deltaTime);
    if (state[SDL_SCANCODE_S])
        camera.processKeyboard(Orientation::kBack, deltaTime);
    if (state[SDL_SCANCODE_A])
        camera.processKeyboard(Orientation::kLeft, deltaTime);
    if (state[SDL_SCANCODE_D])
        camera.processKeyboard(Orientation::kRight, deltaTime);
}

int main() {
    camera.position = bx::Vec3(0.0f, 0.0f, -3.0f);

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

    SDL_SetRelativeMouseMode(SDL_TRUE);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
    .end();
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), layout);

    bgfx::ProgramHandle lightingShader = loadProgram("vs_colors.bin", "fs_colors.bin");
    bgfx::ProgramHandle lightCubeShader = loadProgram("vs_light_cube.bin", "fs_light_cube.bin");

    bgfx::UniformHandle u_viewPos = bgfx::createUniform("u_viewPos", bgfx::UniformType::Vec4);

    bgfx::UniformHandle u_material_ambient = bgfx::createUniform("u_material_ambient", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_material_diffuse = bgfx::createUniform("u_material_diffuse", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_material_specular = bgfx::createUniform("u_material_specular", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_material_shininess = bgfx::createUniform("u_material_shininess", bgfx::UniformType::Vec4);

    bgfx::UniformHandle u_light_position = bgfx::createUniform("u_light_position", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_light_color = bgfx::createUniform("u_light_color", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_light_ambient = bgfx::createUniform("u_light_ambient", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_light_diffuse = bgfx::createUniform("u_light_diffuse", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_light_specular = bgfx::createUniform("u_light_specular", bgfx::UniformType::Vec4);

    float light_color[4] = { 1.0f, 1.0f, 1.0f };
    float light_ambient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float light_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float light_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    bgfx::setUniform(u_light_color, light_color);
    bgfx::setUniform(u_light_ambient, light_ambient);
    bgfx::setUniform(u_light_diffuse, light_diffuse);
    bgfx::setUniform(u_light_specular, light_specular);
    
    float material_ambient[4] = { 0.0f, 0.1f, 0.06f, 1.0f };
    float material_diffuse[4] = { 0.0f, 0.50980392f, 0.50980392f };
    float material_specular[4] = { 0.50196078f, 0.50196078f, 0.50196078f, 1.0f };
    float material_shininess[4] = { 32.0f, 0.0f, 0.0f, 1.0f };
    bgfx::setUniform(u_material_ambient, material_ambient);
    bgfx::setUniform(u_material_diffuse, material_diffuse);
    bgfx::setUniform(u_material_specular, material_specular);
    bgfx::setUniform(u_material_shininess, material_shininess);

    bgfx::setViewRect(0, 0, 0, 800, 600);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x334d4dff);

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
                case SDL_MOUSEMOTION:
                    mouseMoveCallback(window, event.motion.xrel, event.motion.yrel);
                    break;
                case SDL_MOUSEWHEEL:
                    mouseScrollCallback(window, event.wheel.x, event.wheel.y);
                    break;
            }
        }

        auto currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float view[16];
        camera.getViewMatrix(view);

        float proj[16];
        bx::mtxProj(proj, 
            camera.zoom,
            800.0f / 600.0f,
            0.1f, 100.0f,
            bgfx::getCaps()->homogeneousDepth
        );

        bgfx::setViewTransform(0, view, proj);

        float light_pos[4] = { 1.0f + std::sin(currentFrame) * 2.0f, std::cos(currentFrame / 2.0f) * 1.0f, 2.0f, 1.0f };
        bgfx::setUniform(u_light_position, light_pos);

        float viewPos[4] = { camera.position.x, camera.position.y, camera.position.z, 1.0f };
        bgfx::setUniform(u_viewPos, viewPos);

        processInput();

        bgfx::touch(0);

        float transform[16];
        bx::mtxIdentity(transform);
        bgfx::setTransform(transform);
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS);
        bgfx::submit(0, lightingShader);

        bx::mtxSRT(transform,
            0.2f, 0.2f, 0.2f,
            0.0f, 0.0f, 0.0f,
            light_pos[0], light_pos[1], light_pos[2]);
        bgfx::setTransform(transform);
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS);
        bgfx::submit(0, lightCubeShader);

        bgfx::frame();
    }

    bgfx::shutdown();
    SDL_Quit();
    return 0;
}