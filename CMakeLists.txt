cmake_minimum_required(VERSION 3.8)

if(${CMAKE_VERSION} VERSION_LESS 3.15)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
  cmake_policy(VERSION 3.15)
endif()

project(CherrySoda VERSION 0.01
                   DESCRIPTION "A C++ Game Engine Based on BGFX and SDL2"
                   LANGUAGES CXX)

include(cmake/SysDetect.cmake)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

option(CHERRYSODA_BUILD_TOOLS      "Build tools"               ON)
option(CHERRYSODA_BUILD_EXAMPLES   "Build examples"            ON)
option(CHERRYSODA_BUILD_PLAYGROUND "Build playground projects" ON)
option(CHERRYSODA_BUILD_BENCH      "Build benchmark projects"  ON)

# PocketCHIP
if(CHIP)
  add_definitions(-DCHIP)
  add_definitions(-march=armv7-a -mtune=cortex-a8 -ftree-vectorize -ffast-math)
endif()

# ClockworkPi
if(CPI)
  add_definitions(-DCLOCKWORK_PI)
  add_definitions(-mcpu=cortex-a7 -ftree-vectorize -ffast-math)
endif()

set(CHERRYSODA_SIMD "")
if(NOT EMSCRIPTEN)
  if(NOT MSVC)
    if(CHERRYSODA_USE_SSE2)
      set(CHERRYSODA_SIMD "-msse2 -DRAPIDJSON_SSE2")
      message(STATUS "Enabling SSE2")
    elseif(CHERRYSODA_USE_SSE42)
      set(CHERRYSODA_SIMD "-msse4.2 -DRAPIDJSON_SSE42")
      message(STATUS "Enabling SSE4.2")
    elseif(CHERRYSODA_USE_AVX2)
      set(CHERRYSODA_SIMD "-mavx2 -mfma")
      message(STATUS "Enabling AVX2")
    elseif(CHERRYSODA_USE_NEON)
      set(CHERRYSODA_SIMD "-mfpu=neon -mfloat-abi=hard -DRAPIDJSON_NEON")
      message(STATUS "Enabling NEON")
    endif()
  else(NOT MSVC)
    if(CHERRYSODA_USE_SSE2 OR CHERRYSODA_USE_SSE42)
      if(NOT CMAKE_CL_64)
        set(CHERRYSODA_SIMD "/arch:SSE2")
      endif()
    endif()
    if(CHERRYSODA_USE_SSE2)
      set(CHERRYSODA_SIMD "${CHERRYSODA_SIMD} /DRAPIDJSON_SSE2")
      message(STATUS "Enabling SSE2")
    elseif(CHERRYSODA_USE_SSE42)
      set(CHERRYSODA_SIMD "${CHERRYSODA_SIMD} /D__SSE4_2__ /DRAPIDJSON_SSE42")
      message(STATUS "Enabling SSE4.2")
    elseif(CHERRYSODA_USE_AVX2)
      set(CHERRYSODA_SIMD "/arch:AVX2")
      message(STATUS "Enabling AVX2")
    endif()
  endif()
endif()

if(EMSCRIPTEN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 \
                                          -s WASM=1 \
                                          -s NO_EXIT_RUNTIME=1 \
                                          -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1 \
                                          -s OFFSCREENCANVAS_SUPPORT=1 \
                                          -s TOTAL_MEMORY=512MB \
                                          --preload-file assets")
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -O0 -g4 -s ASSERTIONS=2 -s GL_ASSERTIONS=2 -DCHERRYSODA_ENABLE_DEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Oz -s ASSERTIONS=0 -s GL_ASSERTIONS=0 --closure 1") 
  # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Oz -s ASSERTIONS=0 -s GL_ASSERTIONS=0 \
  #                                                         --closure 1 -s WASM_OBJECT_FILES=0 --llvm-lto 1 \
  #                                                         -s AGGRESSIVE_VARIABLE_ELIMINATION=1")
  add_compile_options(-fno-exceptions -fno-rtti)
elseif(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIE ${CHERRYSODA_SIMD}")
  set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -O0 -g -DCHERRYSODA_ENABLE_DEBUG")
  set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -O1 -g -pg -DCHERRYSODA_ENABLE_PROFILE -DTRACY_ENABLE")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
  add_compile_options(-fno-exceptions -fno-rtti)
else(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /D_CRT_SECURE_NO_WARNINGS ${CHERRYSODA_SIMD}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DCHERRYSODA_ENABLE_DEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /DCHERRYSODA_ENABLE_PROFILE /DTRACY_ENABLE")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CHERRYSODA_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CHERRYSODA_ENGINE_PATH ${CHERRYSODA_PROJECT_PATH}/Engine)
set(CHERRYSODA_EXTERNAL_PATH ${CHERRYSODA_PROJECT_PATH}/External)
set(CHERRYSODA_TOOL_PATH ${CHERRYSODA_PROJECT_PATH}/Tools)
set(CHERRYSODA_EXECUTABLE_FOLDER Default)
set(CHERRYSODA_EXECUTABLE_SRC src/main.cpp)
set(CHERRYSODA_SHADER_SRC "")
set(CHERRYSODA_SDL2_PATH ${CHERRYSODA_EXTERNAL_PATH}/SDL2-2.0.12)

function(cherrysoda_set_target_folder target folder)
  set_target_properties(${target} PROPERTIES FOLDER ${folder})
endfunction(cherrysoda_set_target_folder)

function(cherrysoda_set_targets_folder targets folder)
  foreach(target ${targets})
    cherrysoda_set_target_folder(${target} ${folder})
  endforeach()
endfunction(cherrysoda_set_targets_folder)

function(add_cherrysoda_executable executable_name)
  set(program_src "${CHERRYSODA_EXECUTABLE_SRC}")
  set(shader_src "${CHERRYSODA_SHADER_SRC}")
  set(shader_compile_stamp "")
  if(MSVC)
    if(NOT EXISTS ${CHERRYSODA_SDL2_PATH})
      if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
        execute_process(COMMAND python ${CHERRYSODA_TOOL_PATH}/setup_sdl2_for_windows.py
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
      endif()
    endif()
    if(CMAKE_CL_64)
      file(COPY ${CHERRYSODA_SDL2_PATH}/lib/x64/SDL2.dll DESTINATION .)
    else()
      file(COPY ${CHERRYSODA_SDL2_PATH}/lib/x86/SDL2.dll DESTINATION .)
    endif()
  endif()
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  endif()
  if(EMSCRIPTEN)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets AND NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
      file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)
      file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/assets/placeholder)
    endif() 
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/index.html)
      file(COPY index.html DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/index.html)
    else()
      execute_process(COMMAND python ${CHERRYSODA_TOOL_PATH}/generate_index_html.py ${executable_name} ./index.html
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif()
  endif()
  if(NOT "${shader_src}" STREQUAL "")
    if(WINDOWS OR LINUX OR EMSCRIPTEN)
      if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
        set(shader_compile_stamp ${CMAKE_CURRENT_BINARY_DIR}/shader_compile.stamp)
        add_custom_command(OUTPUT ${shader_compile_stamp}
                           COMMAND python ${CHERRYSODA_TOOL_PATH}/compile_shader.py "${CMAKE_CURRENT_SOURCE_DIR}" --file-list \"${shader_src}\"
                           COMMAND ${CMAKE_COMMAND} -E touch ${shader_compile_stamp}
                           COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders ${CMAKE_CURRENT_BINARY_DIR}/assets/shaders
                           DEPENDS ${shader_src}
                           WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
      endif()
    endif()
  endif()
  add_executable(${executable_name} ${program_src} ${shader_src} ${shader_compile_stamp})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${program_src} ${shader_src})
  target_link_libraries(${executable_name} PRIVATE CherrySoda)
  target_include_directories(${executable_name} PRIVATE src)
  cherrysoda_set_target_folder(${executable_name} ${CHERRYSODA_EXECUTABLE_FOLDER})
endfunction(add_cherrysoda_executable)

add_subdirectory(External)
add_subdirectory(Engine)

if(CHERRYSODA_BUILD_TOOLS)
  add_subdirectory(Tools)
endif()

if(CHERRYSODA_BUILD_EXAMPLES)
  add_subdirectory(Examples)
endif()

if(CHERRYSODA_BUILD_PLAYGROUND)
  add_subdirectory(Playground)
endif()

if(CHERRYSODA_BUILD_BENCH)
  add_subdirectory(Bench)
endif()
