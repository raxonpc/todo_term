#pragma once
#include <chrono>
#include <optional>
#include <string>
#include <vector>


namespace todo_term {
using DateType = std::chrono::year_month_day;

class Task {
public:
  Task(const std::string &, bool);
  Task(const std::string &, const DateType &, bool);
  Task(const std::string &, const DateType &);
  Task(const std::string &);

  std::string get_title() const;
  std::optional<DateType> get_deadline() const;
  bool get_done() const;

private:
  std::string m_title{};
  std::optional<DateType> m_deadline{};
  bool m_done{false};
};

class TaskDB {
public:
  // path to sqlite file
  TaskDB(const std::string &);
  ~TaskDB();

  void add_task(const Task &task);
  std::vector<Task> get_tasks() const;

private:
  struct Implementation;
  Implementation *m_impl;
};

std::string DateToString(const DateType &);
DateType StringToDate(const std::string&);

}