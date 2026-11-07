cmake_minimum_required(VERSION 3.20)
project(nectaris_port CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(jsonlib
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(jsonlib)

add_executable(nectaris src/main.cpp)
target_link_libraries(nectaris PRIVATE raylib nlohmann_json::nlohmann_json)
if(WIN32)
  target_link_libraries(nectaris PRIVATE winmm)
endif()
if(MINGW)
  target_link_options(nectaris PRIVATE -static -static-libgcc -static-libstdc++)
endif()

# Make the build folder runnable by double-click: assets next to the exe.
add_custom_command(TARGET nectaris POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_CURRENT_SOURCE_DIR}/../assets"
          "$<TARGET_FILE_DIR:nectaris>/assets"
  COMMENT "Copying assets next to nectaris.exe")
