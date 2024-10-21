#pragma once

#include "json_parser.hpp"
#include "task.hpp"
#include <memory>
#include <string_view>
#include <vector>

class TaskList {
    std::string_view m_tasks_file;
    std::vector<Task> m_tasks;
    std::shared_ptr<JSONParser> m_parser;

public:
    explicit TaskList(std::string_view tasks_file);
    void Save();
    void Add(Task task) noexcept;

private:
    void parse();
};
