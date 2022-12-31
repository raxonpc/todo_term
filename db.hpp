#pragma once
#include <string>
#include <chrono>
#include <optional>

using DateType = std::chrono::year_month_day;
  
class Task {
public:
    Task(const std::string&, const DateType&);
    Task(const std::string&);

    ~Task();

    std::string get_title() const;
    std::optional<DateType> get_deadline() const;
    bool get_done() const;
private:
    struct Implementation;
    Implementation* m_impl;
};

class TaskDB {
private:
    static int m_id_count;
public:
    // path to sqlite file
    TaskDB(const std::string&);
    ~TaskDB();

    void add_task(const Task& task);

private:
    struct Implementation;
    Implementation* m_impl;
};