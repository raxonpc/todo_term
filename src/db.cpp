#include "db.hpp"
#include <sqlite3.h>
#include <string_view>
#include <fmt/core.h>

namespace todo_term {

Task::Task(const std::string& title, bool done) 
: m_title{ title }, m_deadline{ std::nullopt }, m_done{ done } {
}

Task::Task(const std::string& title, const DateType& date, bool done) 
: m_title{ title }, m_deadline{ date }, m_done{ done } {
}


Task::Task(const std::string& title, const DateType& date) 
: m_title{ title }, m_deadline{ date }, m_done{ false } {
}

Task::Task(const std::string& title) 
: m_title{ title }, m_deadline{ std::nullopt }, m_done{ false } {
}


std::string Task::get_title() const { return m_title; }
std::optional<DateType> Task::get_deadline() const { return m_deadline; }
bool Task::get_done() const { return m_done; }


struct TaskDB::Implementation {
    sqlite3* m_db;
    std::string m_file_name;
};

void create_db(sqlite3* db) {
    static constexpr std::string_view create_sql = 
        "create table if not exists Task(id integer primary key, title text not null, deadline text, done integer);";

    char* error_msg = nullptr;
    int create_status = sqlite3_exec(db, create_sql.data(), nullptr, nullptr, &error_msg);

    if(create_status != SQLITE_OK) {
        std::string error_str{ "Unknown internal error" };
        if(error_msg) {
            error_str =  error_msg;
            sqlite3_free(error_msg);
        }
        
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

std::string day_month_str(int day_or_month) {
    if(day_or_month < 10) {
        return std::string{ "0" + std::to_string(day_or_month) };
    }
    return std::to_string(day_or_month);
}

std::string DateToString(const DateType& date) {
    return fmt::format("{}-{}-{}", 
                        static_cast<int>(date.year()),
                        day_month_str(static_cast<unsigned>(date.month())),
                        day_month_str(static_cast<unsigned>(date.day())));
}

DateType StringToDate(const std::string& str) {
    int y, m, d;
    std::sscanf(str.data(), "%d-%d-%d", &y, &m, &d);
    return DateType{    std::chrono::year{ y }, 
                        std::chrono::month{ static_cast<unsigned>(m) }, 
                        std::chrono::day{ static_cast<unsigned>(d)  }
                    };
}

void TaskDB::add_task(const Task& task) {
    std::string insert_sql{};
    if(task.get_deadline()) {
        insert_sql = fmt::format("insert into Task (title, deadline, done) values (\'{}\', \'{}\', FALSE);",
                                    task.get_title(),
                                    DateToString(task.get_deadline().value()));
    }
    else {
        insert_sql = fmt::format("insert into Task (title, done) values (\'{}\', FALSE);",
                                    task.get_title());
    }

    char* error_msg = nullptr;
    int insert_status = sqlite3_exec(m_impl->m_db, insert_sql.c_str(), nullptr, nullptr, &error_msg);
    if(insert_status != SQLITE_OK) {
        std::string error_str{ "Unknown internal error" };
        if(error_msg) {
            error_str =  error_msg;
            sqlite3_free(error_msg);
        }
        
        throw std::runtime_error{ error_str };
    }
    sqlite3_free(error_msg);
}

std::vector<Task> TaskDB::get_tasks() const {
    std::vector<Task> output{};

    sqlite3_stmt *stmt;
    int prepare_status = sqlite3_prepare_v2(m_impl->m_db, "select * from Task order by id", -1, &stmt, nullptr);
    if (prepare_status != SQLITE_OK) {
       throw std::runtime_error{ "ToDo: Internal error (get_tasks)" };
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        output.emplace_back(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            StringToDate(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))),
            static_cast<bool>(sqlite3_column_int(stmt, 3))
        );
    }

    sqlite3_finalize(stmt);

    return output;
}

}