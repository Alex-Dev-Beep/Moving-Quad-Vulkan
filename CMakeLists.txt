cmake_minimum_required(VERSION 3.21)

project(VulkanApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

add_executable(${PROJECT_NAME} WIN32
${SRC_FILES})

find_package(Vulkan REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Vulkan::Vulkan
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
        include
        external/glm
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE STB_IMAGE_IMPLEMENTATION
)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
    add_compile_definitions(
        UNICODE
        _UNICODE
    )
endif()

if (UNIX AND NOT APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_XCB_KHR)
endif()
