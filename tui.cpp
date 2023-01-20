#include "tui.hpp"
#include "ftxui/component/component.hpp"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

Application::Application(const std::string& file_name)
: m_db{ std::make_unique<todo_term::TaskDB>(file_name)}, m_screen{ ScreenInteractive::FitComponent() } {
}

void Application::loop() {
    using namespace std::chrono;

    auto button = Button("add a dummy task", [this](){
        m_db->add_task(todo_term::Task{ std::string { "Lay the table" }, todo_term::DateType{ day{ 23 }/1/2023 }});
    });

    m_screen.Loop(button);
}