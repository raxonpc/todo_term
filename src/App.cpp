#include "App.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include <array>
#include <string_view>

using namespace ftxui;

Application::Application(const std::string &file_name)
:   m_db{std::make_unique<todo_term::TaskDB>(file_name)},
    m_screen{ ScreenInteractive::Fullscreen() } {
    
}

void Application::loop() {
    using namespace std::chrono;

    enum class Option: int {
        tasks = 0,
        create = 1,
        quit = 2
    };

    static std::vector<std::string> entries = {
      "Tasks",
      "Create new task",
      "Quit app"
    };

    while(true) {
        m_screen.Clear();
        int menu_selected = 0;

        MenuOption option = MenuOption::HorizontalAnimated();
        option.on_enter = m_screen.ExitLoopClosure();
        auto menu = Component(Menu(&entries, &menu_selected, &option));

        m_screen.Loop(menu);

        if(static_cast<Option>(menu_selected) == Option::quit) {
            return;
        } else if(static_cast<Option>(menu_selected) == Option::create) {
            m_db->add_task(todo_term::Task{std::string{"Lay the table"},
                            todo_term::DateType{day{23} / 1 / 2023}});
        }
    }
}