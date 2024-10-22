#include "task_list.hpp"
#include "json_parser.hpp"
#include "task.hpp"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string_view>

TaskList::TaskList(std::string_view tasks_file)
    : m_tasks_file(tasks_file) {
    if (!std::filesystem::exists(m_tasks_file)) {
        std::ofstream file(m_tasks_file.data());
        file << "[]";
        file.close();
    }
    m_parser = JSONParser();
    parse();
}

void TaskList::Save() {
    std::stringstream ss;
    ss << "[";
    for (auto it = m_tasks.begin(); it != m_tasks.end(); it++) {
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

void TaskList::PrintAllTasks() noexcept {
    for (const auto &task : m_tasks) {
        task.Print(std::cout);
        std::cout << "\n";
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
