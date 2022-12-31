cmake_minimum_required(VERSION 3.10)
project(todo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TODO_SRC
    db.cpp
)
add_library(todo_lib ${TODO_SRC})

find_package(fmt)
target_link_libraries(todo_lib sqlite3 fmt::fmt)

add_executable(todo main.cpp)
target_link_libraries(todo todo_lib)
