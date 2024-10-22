#include "task_list.hpp"
#include "json_parser.hpp"
#include "task.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string_view>

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

void TaskList::Add(Task task) noexcept {
    task.id = static_cast<int>(m_tasks.size()) + 1;
    m_tasks.emplace_back(std::move(task));
}
auto TaskList::CountBy(const TaskStatus &status) -> size_t {
    auto res = GetTasksByStatus(status);
    return res.size();
}

auto TaskList::GetTasksById(int id) -> std::vector<Task> {
    auto filter = [id](const Task& t) {return t.id == id;};
    return GetTasksBy(filter);
}
auto TaskList::GetTasksByStatus(const TaskStatus &status) -> std::vector<Task> {
    std::vector<Task> result;
    auto filter = [status](const Task &t) { return t.status == status; };
    std::ranges::copy_if(m_tasks.begin(), m_tasks.end(), std::back_inserter(result), filter);
    return result;
}
auto TaskList::GetTasksBy(const std::function<bool(const Task &)> &filter_func)
    -> std::vector<Task> {
    std::vector<Task> result;
    std::ranges::copy_if(m_tasks.begin(), m_tasks.end(), std::back_inserter(result), filter_func);
    return result;
}
void TaskList::Update(const Task &task) {
    auto t = std::ranges::find_if(m_tasks.begin(), m_tasks.end(), [task](const auto &t) {
        return task.id == t.id;
    });
    if (t != m_tasks.end()) {
        // Меняем статус и Описание
        t->description = task.description;
        t->status = task.status;
        t->BeforeUpdate();
    } else {
        auto tt = task;
        tt.id = -1;
        tt.BeforeCreate();
    }
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
