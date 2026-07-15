cmake_minimum_required(VERSION 3.24...4.0)
project(TK LANGUAGES C CXX)

set(VENDOR_DIR "vendor")
set(INCLUDE_DIR "include")
set(SOURCE_DIR "src")
set(TK_SOURCE_DIR ${SOURCE_DIR}/TK)
set(DEMO_SOURCE_DIR ${SOURCE_DIR}/Demo)
set(VENDOR_INCLUDE_PATHS ${VENDOR_DIR}/SDL/include ${VENDOR_DIR}/glad/include ${VENDOR_DIR}/cglm/include ${VENDOR_DIR}/volk)


set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)



add_subdirectory(${VENDOR_DIR}/SDL)


add_library(glad STATIC)
target_sources(glad PRIVATE ${VENDOR_DIR}/glad/src/glad.c)
target_include_directories(glad PRIVATE ${VENDOR_INCLUDE_PATHS})



add_library(TK STATIC)
target_sources(TK PRIVATE
    ${TK_SOURCE_DIR}/Allocator.c
    ${TK_SOURCE_DIR}/Entity.c
    ${TK_SOURCE_DIR}/Properties.c
    ${TK_SOURCE_DIR}/String.c
    ${TK_SOURCE_DIR}/Renderer/OpenGL.c
    ${TK_SOURCE_DIR}/Platform/SDL/Common.c
    ${TK_SOURCE_DIR}/Platform/SDL/Window.c
    ${TK_SOURCE_DIR}/Platform/SDL/Update.c)
target_link_libraries(TK PRIVATE m SDL3 glad)
target_include_directories(TK PRIVATE ${INCLUDE_DIR} ${SOURCE_DIR} ${VENDOR_INCLUDE_PATHS})
target_compile_options(TK PRIVATE -Wno-pragma-once-outside-header -Wall -Wno-missing-braces)



add_executable(Demo)
target_sources(Demo PRIVATE ${DEMO_SOURCE_DIR}/Demo.c)
target_link_libraries(Demo PRIVATE TK)
target_include_directories(Demo PRIVATE ${INCLUDE_DIR} ${SOURCE_DIR} ${VENDOR_INCLUDE_PATHS})
target_compile_options(Demo PRIVATE -Wno-pragma-once-outside-header -Wall -Wno-missing-braces)