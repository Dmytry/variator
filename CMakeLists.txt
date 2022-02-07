cmake_minimum_required(VERSION 3.5.0)
project(SimpleMap VERSION 0.1.0)
include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wno-unused)

add_executable(variator variator_main.cpp)
# target_link_libraries(variator)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_custom_target(
        clangformat
        COMMAND /usr/bin/clang-format-10
        -sort-includes=false
        -style='{BasedOnStyle: Google, IndentWidth: 4}'
        -i
        ${PROJECT_SOURCE_DIR}/variator_main.cpp
)
