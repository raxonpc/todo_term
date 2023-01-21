#include "App.hpp"
#include "db.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/flexbox_config.hpp"
#include "ftxui/dom/node.hpp"

#include <array>
#include <functional>
#include <string_view>

using namespace ftxui;

Application::Application(const std::string &file_name)
:   m_db{std::make_unique<todo_term::TaskDB>(file_name)},
    m_screen{ ScreenInteractive::Fullscreen() } {
    
}

void Application::create_menu() {
    m_screen.Clear();

    std::string task_title{};
    std::string task_deadline{};

    auto input_task_title = Input(&task_title, "title");
    auto input_task_deadline = Input(&task_deadline, "deadline");

    auto submit_button = Button("Submit", [this, &task_title, &task_deadline]() {
                m_db->add_task(todo_term::Task{task_title, todo_term::StringToDate(task_deadline)});
            });
    auto exit_button = Button("Exit", [this] {
        m_screen.ExitLoopClosure()();
    });

    auto component = Container::Vertical({
        input_task_title,
        input_task_deadline,
        submit_button,
        exit_button
    });

    auto config = FlexboxConfig{};
    config.justify_content = FlexboxConfig::JustifyContent::Center;
    auto renderer = Renderer(component, [&] {
    return flexbox(
        {
             window(text("Create new task"), vbox(
                hbox(text(" Task title : "), input_task_title->Render()),
                hbox(text(" Task deadline : "), input_task_deadline->Render()),
                submit_button->Render(),
                exit_button->Render()
            ))
        },
        config
    );
  });
  m_screen.Loop(renderer);
}

void Application::task_menu() {
    auto tasks = m_db->get_tasks();

    Elements task_elements = Elements{};

    for(const auto& task : tasks) {
        auto deadline = task.get_deadline();
        std::string deadline_str{ "no deadline" };
        if(deadline) {
            deadline_str = todo_term::DateToString(*deadline);
        }

        auto label = task.get_done() ? "Done" : "Undone";
        auto color_done = task.get_done() ? Color::Green : Color::Red;
        task_elements.push_back(
            vbox(
                hbox(text("Title: "), text(task.get_title())),
                hbox(text("Deadline: "), text(deadline_str)),
                hbox(text("Status: "), color(color_done, text(label)))
             ) | border | size(WIDTH, LESS_THAN, 30)
        );
    }

    auto exit_button = Button("Exit", [this] {
        m_screen.ExitLoopClosure()();
    }) | size(WIDTH, LESS_THAN, 30);

    auto config = FlexboxConfig{};
    config.justify_content = FlexboxConfig::JustifyContent::Center;
    auto renderer = Renderer(Component{ exit_button }, [&]() {
    return flexbox(
        { vbox(
            task_elements,
            exit_button->Render() | focus
        ) | yflex | yframe },
        config);
    });

    m_screen.Loop(renderer);
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

    m_screen.Clear();

    MenuOption option = MenuOption::HorizontalAnimated();
    option.on_enter = [this]() {
        switch(static_cast<Option>(menu_selected)) {
            case Option::quit:
                m_screen.ExitLoopClosure()();
                break;
            case Option::create:
                create_menu();
                break;
            case Option::tasks:
                task_menu();
                break;
        }
    };
    auto menu = Component(Menu(&entries, &menu_selected, &option));

    m_screen.Loop(menu);
}