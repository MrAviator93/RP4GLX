cmake_minimum_required(VERSION "3.20.0")

project(RPIOpenES VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
            "-Wall" "-Wextra" "-Werror"
            "-Wnon-virtual-dtor" "-Wcast-align" "-Wunused"
            "-Woverloaded-virtual"
            "-fexceptions" "-pedantic"
            "-O2" "-no-pie"
    )
endif()

find_package(X11 REQUIRED)
find_package(Threads REQUIRED)

# Source files
set(SOURCES
    main.cpp
    GlxRender.cpp
    GlxDevice.cpp
    GlxContext.cpp
    Window.cpp
)

# Libraries to link against
set(LIBS
    X11
    GL
    EGL
    GLESv2
    ${X11_LIBRARIES}
    Threads::Threads
)

# Add the executable
add_executable(main.out ${SOURCES})

# Copy shader assets into the build output directory
set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../data/shaders")
set(SHADER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/data/shaders")
add_custom_command(TARGET main.out POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/data"
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${SHADER_SOURCE_DIR}" "${SHADER_OUTPUT_DIR}"
)

# Link libraries
target_link_libraries(main.out ${LIBS})
