cmake_minimum_required(VERSION 3.16)
project(SFMLTEST LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(REFERENCE_DIR "${CMAKE_SOURCE_DIR}/references")

configure_file(
    "${CMAKE_SOURCE_DIR}/config/ReferenceConfig.h.in"
    "${CMAKE_BINARY_DIR}/ReferenceConfig.h"
)


option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

include_directories("${CMAKE_SOURCE_DIR}/include/engineIncludes" "${CMAKE_SOURCE_DIR}/include/guiIncludes" "${CMAKE_BINARY_DIR}")
add_executable(main src/engine/mainChess.cpp
 src/engine/chessEngine0.0.cpp
 src/engine/engine.cpp
 src/engine/Engine0.1.cpp 
 src/engine/Engine01Wrapper.cpp
 src/engine/Engine02.cpp 
 src/engine/engineRandom.cpp 
 src/engine/engineTest.cpp
 src/engine/EngineWrapper.cpp
 src/gui/Piece.cpp
 src/gui/Board.cpp
 src/gui/Gui.cpp
 src/gui/Promotion.cpp
 src/gui/MoveGeneration.cpp
 src/gui/Button.cpp
 src/gui/StartScreen.cpp
 )

# Link SFML libraries
target_link_libraries(main PRIVATE sfml-graphics sfml-window sfml-system)

# Set C++ standard to C++17
target_compile_features(main PRIVATE cxx_std_17)

if(WIN32)
    # Copy necessary DLLs (e.g., openal32.dll) to the output directory
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
               # "${SFML_BINARY_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll"
               "C:/Users/Adam/Documents/SFML/SFML-2.6.1/bin/openal32.dll"
                $<TARGET_FILE_DIR:main>
        VERBATIM
    )
endif()