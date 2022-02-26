set(LEARNBGFX_SOURCES
    src/main.cc)

add_executable(learnbgfx ${LEARNBGFX_SOURCES})

find_package(SDL2 REQUIRED)
find_package(bgfx REQUIRED)

target_link_libraries(learnbgfx 
    ${SDL2_LIBRARIES}
    bgfx::bgfx    
)

target_include_directories(learnbgfx PRIVATE
    ${SDL2_INCLUDE_DIRS}
)