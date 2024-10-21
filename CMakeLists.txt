cmake_minimum_required(VERSION 3.20)


if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

project(task-cli VERSION 0.1.0 LANGUAGES CXX)


include(FetchContent)

FetchContent_Declare(
  glaze
  GIT_REPOSITORY https://github.com/stephenberry/glaze.git
  GIT_TAG main
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glaze)


file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE headers src/*.h src/*.hpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources} ${headers})

add_executable(${PROJECT_NAME} ${sources} ${headers})

target_link_libraries(${PROJECT_NAME} PRIVATE glaze::glaze)
