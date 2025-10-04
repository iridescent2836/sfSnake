cmake_minimum_required(VERSION 3.16)
project(sfsnake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://gitee.com/mirrors_laurentgomila/sfml.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# add the folder that cmakelist is in as the src file 
aux_source_directory(. SRC_LIST)
add_executable(sfsnake ${SRC_LIST})

# link the sfml libraries to the sfsnake executable
target_link_libraries(sfsnake PRIVATE sfml-graphics sfml-audio sfml-system)
target_compile_features(sfsnake PRIVATE cxx_std_17)

# Copy OpenAL DLL to the output directory
if(WIN32)
    add_custom_command(
        TARGET sfsnake
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:sfsnake>
        VERBATIM)
endif()

install(TARGETS sfsnake)