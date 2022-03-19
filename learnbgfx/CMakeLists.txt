include(BgfxHelper)

set(LEARNBGFX_SOURCES
    src/main.cc)
add_executable(learnbgfx ${LEARNBGFX_SOURCES})

set(LEARNBGFX_SHADERS
    shaders/colors/vs_colors.sc
    shaders/colors/fs_colors.sc
    shaders/light_cube/vs_light_cube.sc
    shaders/light_cube/fs_light_cube.sc)
add_bgfx_shader(learnbgfx ${LEARNBGFX_SHADERS})

set(LEARNBGFX_TEXTURES
    textures/awesomeface.png    
    textures/container.jpg
    textures/container2.png
    textures/container2_specular.png
    textures/lighting_maps_specular_color.png
    textures/matrix.jpg)
add_bgfx_texture(learnbgfx ${LEARNBGFX_TEXTURES})

find_package(SDL2 REQUIRED)
find_package(bgfx REQUIRED)

target_link_libraries(learnbgfx 
    ${SDL2_LIBRARIES}
    bgfx::bgfx
)

target_include_directories(learnbgfx PRIVATE
    ${SDL2_INCLUDE_DIRS}
)