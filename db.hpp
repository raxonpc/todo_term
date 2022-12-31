#pragma once
#include <string>
#include <chrono>
#include <optional>

using DateType = std::chrono::year_month_day;
  
struct Task {
    Task() = default;

    std::string m_title{};
    std::optional<DateType> m_deadline{};
    bool m_is_done{ false };
};

class TaskDB {
public:
    // path to sqlite file
    TaskDB(const std::string&);
    ~TaskDB();

    void add_task(const Task& task);

private:
    struct Implementation;
    Implementation* m_impl;
};