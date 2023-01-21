#pragma once

#include "db.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <memory>

class Application {
public:
  Application(const std::string &);

  void loop();

private:
  std::unique_ptr<todo_term::TaskDB> m_db;
  ftxui::ScreenInteractive m_screen;
  int menu_selected{ 0 };

  void create_menu();
  void task_menu();
};