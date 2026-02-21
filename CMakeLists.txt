
cmake_minimum_required(VERSION 3.20)
project(CocosCppGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Paths ---
set(COCOS2D_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/external/cocos2d)
set(GAME_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GAME_ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)

# --- Add the engine (requires submodules initialized) ---
add_subdirectory(${COCOS2D_ROOT} ${CMAKE_BINARY_DIR}/cocos2d)

# --- Your target (use your actual source list) ---
add_executable(CocosCppGame
    ${GAME_SRC_DIR}/main.cpp
    ${GAME_SRC_DIR}/AppDelegate.cpp
    ${GAME_SRC_DIR}/AppDelegate.h
    ${GAME_SRC_DIR}/MainScene.cpp
    ${GAME_SRC_DIR}/MainScene.h
    # add your existing files here…
)

target_link_libraries(CocosCppGame PRIVATE cocos2d)

target_include_directories(CocosCppGame PRIVATE
    ${COCOS2D_ROOT}/cocos
    ${COCOS2D_ROOT}/external
    ${GAME_SRC_DIR}
)

# --- Copy assets next to the built executable ---
add_custom_command(TARGET CocosCppGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${GAME_ASSETS_DIR}
            $<TARGET_FILE_DIR:CocosCppGame>/assets
    COMMENT "Copying assets to runtime directory"
)
