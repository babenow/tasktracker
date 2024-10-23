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
    auto nextId() -> int;
public:
    explicit TaskList(std::string_view tasks_file);
    void Save();
    void Add(std::shared_ptr<Task> task) noexcept;
    void Update(const Task &task) noexcept;
    void Delete(int id) noexcept;

    auto empty() const -> bool { return m_tasks.empty(); }
    auto CountBy(const TaskStatus& status) const -> size_t;
    [[nodiscard]] auto GetAll() const -> std::vector<Task> { return m_tasks; };
    [[nodiscard]] auto GetTasksById(int id) const -> std::optional<Task>;
    [[nodiscard]] auto GetTasksByStatus(const TaskStatus &status) const -> std::vector<Task>;
    [[nodiscard]] auto GetTasksBy(const std::function<bool(const Task &)> &filter_func) const
        -> std::vector<Task>;


private:
    void parse();
};
