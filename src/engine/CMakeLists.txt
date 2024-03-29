﻿
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE omnia_engine_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE omnia_engine_headers "[a-zA-Z]*.hpp")

add_library(omnia_engine SHARED
	  ${omnia_engine_sources}
	  ${omnia_engine_headers})

add_dependencies (omnia_engine
	yaml_cpp
	glad
	libretti
)

 add_definitions(/bigobj)

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../../thirdparty/SDL2-2.28.5/lib/x64)
find_library(SDL2_Mixer SDL2_mixer.lib PATHS ../../thirdparty/SDL2_mixer-2.6.3/lib/x64)
find_library(SDL2_TTF SDL2_ttf.lib PATHS ../../thirdparty/SDL2_ttf-2.0.18/lib/x64)
find_library(Python37 python37.lib PATHS ../../thirdparty/python37/lib)
find_library(ZLIB zlib.lib PATHS ../../thirdparty/zlib-1.2.11/lib)


target_include_directories(omnia_engine PUBLIC
		../../thirdparty/SDL2-2.28.5/include
		../../thirdparty/SDL2_mixer-2.6.3/include
		../../thirdparty/SDL2_ttf-2.0.18/include
		../../thirdparty/yaml-cpp-0.6.3/include
		../../thirdparty/python37/include
		../../thirdparty/pybind11-2.11.1/include
		../../thirdparty/glad/include
		../../thirdparty/glm-0.9.9.8/glm
		../../thirdparty/tinygltf-2.8.18/include
		../../thirdparty/zlib-1.2.11/include
		../../thirdparty/stb_image_write-1.16/include
		../../thirdparty/stb_image-2.27/include
		../../thirdparty/libretti/include
		.
)

target_link_libraries (omnia_engine
		yaml_cpp
		glad
		libretti
		${SDL2}
		${SDL2_Mixer}
		${SDL2_TTF}
		${Python37}
		${ZLIB}

)

add_definitions(-D_OMNIA_ENGINE_EXPORTS)

add_custom_command(TARGET omnia_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:omnia_engine> ${PROJECT_BINARY_DIR}/src/main/omnia_engine.dll
)