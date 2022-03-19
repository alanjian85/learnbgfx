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

void WindowResizeCallback(SDL_Window* window, int width, int height) {
    bgfx::reset(width, height);
    bgfx::setViewRect(0, 0, 0, width, height);
}

void MouseMoveCallback(SDL_Window* window, Sint32 xrel, Sint32 yrel) {
    camera.processMouseMovement(xrel, -yrel);
}

void MouseScrollCallback(SDL_Window* window, Sint32 x, Sint32 y) {
    camera.processMouseScroll(y);
}

void ProcessInput() {
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
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    bx::Vec3 cube_positions[] = {
        bx::Vec3( 0.0f,  0.0f,  0.0f),
        bx::Vec3( 2.0f,  5.0f, -15.0f),
        bx::Vec3(-1.5f, -2.2f, -2.5f),
        bx::Vec3(-3.8f, -2.0f, -12.3f),
        bx::Vec3( 2.4f, -0.4f, -3.5f),
        bx::Vec3(-1.7f,  3.0f, -7.5f),
        bx::Vec3( 1.3f, -2.0f, -2.5f),
        bx::Vec3( 1.5f,  2.0f, -2.5f),
        bx::Vec3( 1.5f,  0.2f, -1.5f),
        bx::Vec3(-1.3f,  1.0f, -1.5f)
    };

    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
    .end();
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), layout);

    bgfx::ProgramHandle lighting_shader = LoadProgram("vs_colors.bin", "fs_colors.bin");
    bgfx::ProgramHandle light_cube_shader = LoadProgram("vs_light_cube.bin", "fs_light_cube.bin");

    bgfx::UniformHandle u_view_pos = bgfx::createUniform("u_viewPos", bgfx::UniformType::Vec4);

    bgfx::UniformHandle s_material_diffuse = bgfx::createUniform("s_materialDiffuse", bgfx::UniformType::Sampler);
    bgfx::UniformHandle s_material_specular = bgfx::createUniform("s_materialSpecular", bgfx::UniformType::Sampler);
    bgfx::UniformHandle u_material_shininess = bgfx::createUniform("u_materialShininess", bgfx::UniformType::Vec4);

    bgfx::UniformHandle u_light_position = bgfx::createUniform("u_lightPosition", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_light_direction = bgfx::createUniform("u_lightDirection", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_light_ambient = bgfx::createUniform("u_lightAmbient", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_light_diffuse = bgfx::createUniform("u_lightDiffuse", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_light_specular = bgfx::createUniform("u_lightSpecular", bgfx::UniformType::Vec4);
    bgfx::UniformHandle u_light_constants = bgfx::createUniform("u_lightConstants", bgfx::UniformType::Vec4);

    float light_ambient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float light_diffuse[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float light_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float light_constants[4] = { 1.0f, 0.09f, 0.032f, bx::cos(bx::toRad(12.5f)) };
    bgfx::setUniform(u_light_ambient, light_ambient);
    bgfx::setUniform(u_light_diffuse, light_diffuse);
    bgfx::setUniform(u_light_specular, light_specular);
    bgfx::setUniform(u_light_constants, light_constants);
    
    bgfx::TextureHandle diffuse_map = LoadTexture("container2.dds");
    bgfx::TextureHandle specular_map = LoadTexture("container2_specular.dds");
    float material_shininess[4] = { 32.0f, 0.0f, 0.0f, 1.0f };
    bgfx::setTexture(0, s_material_diffuse, diffuse_map);
    bgfx::setTexture(1, s_material_specular, specular_map);
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
                            WindowResizeCallback(window, event.window.data1, event.window.data2);
                            break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    MouseMoveCallback(window, event.motion.xrel, event.motion.yrel);
                    break;
                case SDL_MOUSEWHEEL:
                    MouseScrollCallback(window, event.wheel.x, event.wheel.y);
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

        float light_position[4] = { camera.position.x, camera.position.y, camera.position.z, 1.0f };
        float light_direction[4] = { camera.direction().x, camera.direction().y, camera.direction().z, 1.0f };
        bgfx::setUniform(u_light_position, light_position);
        bgfx::setUniform(u_light_direction, light_direction);

        float view_pos[4] = { camera.position.x, camera.position.y, camera.position.z, 1.0f };
        bgfx::setUniform(u_view_pos, view_pos);

        ProcessInput();

        bgfx::touch(0);

        for(unsigned int i = 0; i < 10; i++) {
            float transform[16];
            float angle = 20.0f * i;
            bx::mtxSRT(transform,
                1.0f, 1.0f, 1.0f,
                -bx::toRad(angle), 0.3f * -bx::toRad(angle), 0.5f * -bx::toRad(angle),
                cube_positions[i].x, cube_positions[i].y, -cube_positions[i].z
            );
            bgfx::setTransform(transform);
            bgfx::setVertexBuffer(0, vbh);
            bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS);
            bgfx::submit(0, lighting_shader);
        }

        bgfx::frame();
    }

    bgfx::shutdown();
    SDL_Quit();
    return 0;
}