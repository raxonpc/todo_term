#include "db.hpp"
#include <sqlite3.h>
#include <string_view>
#include <fmt/core.h>
#include <iostream>

struct Task::Implementation {
    std::string m_title{};
    std::optional<DateType> m_deadline{};
    bool m_done{ false };
};

Task::Task(const std::string& title, const DateType& date) 
: m_impl{ new Implementation } {
    m_impl->m_title = title;
    m_impl->m_deadline = date;
}

Task::Task(const std::string& title) 
: m_impl{ new Implementation } {
    m_impl->m_title = title;
    m_impl->m_deadline = std::nullopt;
}

Task::~Task() {
    delete m_impl;
}

std::string Task::get_title() const { return m_impl->m_title; }
std::optional<DateType> Task::get_deadline() const { return m_impl->m_deadline; }
bool Task::get_done() const { return m_impl->m_done; }


struct TaskDB::Implementation {
    sqlite3* m_db;
    std::string m_file_name;
};

void create_db(sqlite3* db) {
    static constexpr std::string_view create_sql = 
                        "CREATE TABLE TASK("
                        "ID         INT     PRIMARY KEY     NOT NULL, "
                        "TITLE      TEXT                    NOT NULL, "
                        "DEADLINE   TEXT,"
                        "DONE       INTEGER );";

    char* error_msg = nullptr;
    int create_status = sqlite3_exec(db, create_sql.data(), nullptr, 0, &error_msg);

    if(create_status != SQLITE_OK) {
        std::string error_str{ error_msg };
        sqlite3_free(error_msg);
        throw std::runtime_error{ error_str };
    }
}

TaskDB::TaskDB(const std::string& file_name)
    : m_impl{ new Implementation } {
        m_impl->m_file_name = file_name;
        int db_status = sqlite3_open(m_impl->m_file_name.c_str(), &m_impl->m_db);

        if (db_status) {
            throw std::runtime_error{ sqlite3_errmsg(m_impl->m_db) };
        }

        create_db(m_impl->m_db);
    }

TaskDB::~TaskDB() {
    sqlite3_close(m_impl->m_db);
    delete m_impl;
}

int TaskDB::m_id_count{ 0 };

std::string day_month_str(int day_or_month) {
    if(day_or_month < 10) {
        return std::string{ "0" + std::to_string(day_or_month) };
    }
    return std::to_string(day_or_month);
}

std::string DateToString(const DateType& date) {
    auto str = fmt::format("{}-{}-{}", 
                        static_cast<int>(date.year()),
                        day_month_str(static_cast<unsigned>(date.month())),
                        day_month_str(static_cast<unsigned>(date.day())));
    std::cerr << str << '\n';
    return str;
}

void TaskDB::add_task(const Task& task) {
    std::string insert_sql{};
    if(task.get_deadline()) {
        insert_sql = fmt::format("INSERT INTO TASK VALUES({}, \'{}\', \'{}\', FALSE);", 
                                    m_id_count++,
                                    task.get_title(),
                                    DateToString(task.get_deadline().value()));
    }

    char* error_msg = nullptr;
    int insert_status = sqlite3_exec(m_impl->m_db, insert_sql.c_str(), nullptr, nullptr, &error_msg);
    if(insert_status != SQLITE_OK) {
        std::string error_str{ error_msg };
        sqlite3_free(error_msg);
        throw std::runtime_error{ error_str };
    }
    sqlite3_free(error_msg);
}