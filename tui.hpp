#pragma once

#include <memory>
#include "db.hpp"
#include "ftxui/component/screen_interactive.hpp"

class Application {
public:
    Application(const std::string&);

    void loop();

private:
    std::unique_ptr<todo_term::TaskDB> m_db;
    ftxui::ScreenInteractive m_screen;
};