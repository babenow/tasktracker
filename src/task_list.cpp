#include "task_list.hpp"
#include "json_parser.hpp"
#include "task.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string_view>

auto TaskList::nextId() -> int {
    int next = 0;
    for (const auto &t : m_tasks) {
        next = std::max(t.id, next);
    }
    return next + 1;
}

TaskList::TaskList(std::string_view tasks_file)
    : m_tasks_file(tasks_file) {
    if (!std::filesystem::exists(m_tasks_file)) {
        std::ofstream file(m_tasks_file);
        file << "[]";
        file.close();
    }
    m_parser = JSONParser();
    parse();
}

void TaskList::Save() {
    std::stringstream ss;
    ss << "[";
    for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it) {
        if (it != m_tasks.begin()) {
            ss << ",";
        }
        ss << it->ToJsonString();
    }
    ss << "]";

    std::ofstream file(m_tasks_file.data());
    file << ss.str();
    file.close();
}

auto TaskList::Add(const std::shared_ptr<Task> &task) noexcept -> void {
    task->id = nextId();
    task->BeforeCreate();
    m_tasks.emplace_back(std::move(*task));
}
auto TaskList::CountBy(const TaskStatus &status) const -> size_t {
    const auto RES = GetTasksByStatus(status);
    return RES.size();
}

auto TaskList::GetTasksById(int id) const -> std::optional<Task> {
    auto filter = [id](const Task &t) { return t.id == id; };
    auto task = std::ranges::find_if(m_tasks.begin(), m_tasks.end(), filter);

    if (task == m_tasks.end()) {
        return std::nullopt;
    }
    return *task;
}

auto TaskList::GetTasksByStatus(const TaskStatus &status) const -> std::vector<Task> {
    std::vector<Task> result;
    auto filter = [status](const Task &t) { return t.status == status; };
    std::ranges::copy_if(m_tasks.begin(), m_tasks.end(), std::back_inserter(result), filter);
    return result;
}
auto TaskList::GetTasksBy(const std::function<bool(const Task &)> &filter_func) const
    -> std::vector<Task> {
    std::vector<Task> result;
    std::ranges::copy_if(m_tasks.begin(), m_tasks.end(), std::back_inserter(result), filter_func);
    return result;
}
void TaskList::Update(const Task &task) noexcept {
    const auto T = std::ranges::find_if(m_tasks.begin(), m_tasks.end(), [task](const auto &t) {
        return task.id == t.id;
    });
    if (T != m_tasks.end()) {
        // Меняем статус и Описание
        T->description = task.description;
        T->status = task.status;
        T->BeforeUpdate();
    } else {
        auto tt = task;
        tt.id = -1;
        tt.BeforeCreate();
    }
}

void TaskList::Delete(int id) noexcept {
    auto ok_func = [&](const Task &t) { return id == t.id; };
    m_tasks.erase(std::ranges::remove_if(m_tasks, ok_func).begin(), m_tasks.end());
}

void TaskList::parse() {
    std::ifstream file(m_tasks_file.data());
    std::string str;
    std::string json;
    while (std::getline(file, str)) {
        json += str;
        json.push_back('\n');
    }
    file.close();

    m_parser.setData(json);
    m_tasks = m_parser.parseTaskList();
}
