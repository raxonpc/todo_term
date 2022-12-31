#include "db.hpp"
#include <sqlite3.h>

struct TaskDB::Implementation {
    sqlite3* m_db;
    std::string m_file_name;
};

TaskDB::TaskDB(const std::string& file_name)
    : m_impl{ new Implementation } {
        m_impl->m_file_name = file_name;
        int db_status = sqlite3_open(m_impl->m_file_name.c_str(), &m_impl->m_db);

        if (db_status) {
            throw std::runtime_error{ sqlite3_errmsg(m_impl->m_db) };
        }
    }

TaskDB::~TaskDB() {
    sqlite3_close(m_impl->m_db);
    delete m_impl;
}