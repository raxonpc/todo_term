#include "App.hpp"

int main(int argc, char *argv[]) {
  Application app("tasks.db");
  app.loop();

  return 0;
}