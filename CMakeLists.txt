cmake_minimum_required(VERSION 3.10)
project(todo_term)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TODO_SRC
    db.cpp
)
add_library(todo_lib STATIC ${TODO_SRC})

find_package(fmt)
target_link_libraries(todo_lib sqlite3 fmt::fmt)

# FTXUI
include(FetchContent)
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v3.0.0
)
FetchContent_MakeAvailable(ftxui)


add_executable(todo main.cpp tui.cpp)
target_link_libraries(todo PRIVATE  todo_lib ftxui::screen ftxui::dom ftxui::component)
