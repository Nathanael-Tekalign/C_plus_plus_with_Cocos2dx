cmake_minimum_required(VERSION 3.20)
project(CocosCppGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# If vendoring cocos2d-x as a submodule under external/cocos2d
# add_subdirectory(external/cocos2d)
git submodule add https://github.com/cocos2d/cocos2d-x.git external/cocos2d
git submodule update --init --recursive

# Or, if installed separately, point CMake at it:
# set(COCOS2D_ROOT "/path/to/cocos2d-x")
# add_subdirectory(${COCOS2D_ROOT} ${CMAKE_BINARY_DIR}/cocos2d)

add_executable(CocosCppGame
    src/main.cpp
    src/AppDelegate.cpp
    src/MainScene.cpp
    src/AppDelegate.h
    src/MainScene.h
)

# Link against cocos2d
# target_link_libraries(CocosCppGame PRIVATE cocos2d)

target_include_directories(CocosCppGame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    # ${COCOS2D_ROOT}/cocos    # if referencing installed engine
)

# Copy assets next to the binary (simple cross‑platform approach)
add_custom_command(TARGET CocosCppGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:CocosCppGame>/assets
)
