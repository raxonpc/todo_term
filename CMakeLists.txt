cmake_minimum_required(VERSION 3.10)
project(todo)

set(CMAKE_CXX_STANDARD 20)

set(TODO_SRC
    db.cpp
)
add_library(todo_lib ${TODO_SRC})
target_link_libraries(todo_lib sqlite3)

add_executable(todo main.cpp)
target_link_libraries(todo todo_lib)
