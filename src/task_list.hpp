#pragma once

#include "json_parser.hpp"
#include "task.hpp"
#include <functional>
#include <string_view>
#include <vector>

class TaskList {
    std::string m_tasks_file;
    std::vector<Task> m_tasks;
    JSONParser m_parser;

public:
    explicit TaskList(std::string_view tasks_file);
    void Save();
    void Add(Task task) noexcept;
    auto empty() const -> bool { return m_tasks.empty(); }
    auto CountBy(const TaskStatus& status) -> size_t;
    [[nodiscard]] auto GetAll() const -> std::vector<Task> { return m_tasks; };
    [[nodiscard]] auto GetTasksById(int id) -> std::vector<Task>;
    [[nodiscard]] auto GetTasksByStatus(const TaskStatus &status) -> std::vector<Task>;
    [[nodiscard]] auto GetTasksBy(const std::function<bool(const Task &)> &filter_func)
        -> std::vector<Task>;
    void Update(const Task &task);

private:
    void parse();
};
