cmake_minimum_required(VERSION 3.14)
project(todo_term)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# DEPENDENCIES

find_package(fmt)

# FTXUI
include(FetchContent)
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v3.0.0
)
FetchContent_MakeAvailable(ftxui)

# APP

set(TODO_SRC
    src/db.cpp
    src/main.cpp
    src/App.cpp
)
add_executable(todo ${TODO_SRC})

target_link_libraries(todo PRIVATE sqlite3 fmt::fmt ftxui::screen ftxui::dom ftxui::component)
