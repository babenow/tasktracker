#include "commands.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

static const std::unordered_map<std::string, TaskStatus> SUPPORTED_LIST_FILTERS{
    {"todo", TaskStatus::TODO},
    {"tbd", TaskStatus::TODO},
    {"done", TaskStatus::DONE},
    {"is-done", TaskStatus::DONE},
    {"in-progress", TaskStatus::IN_PROGRESS},
    {"progress", TaskStatus::IN_PROGRESS},
};

Command::Command(std::vector<std::string> args)
    : m_arguments(std::move(args)) {};

auto Command::GetTaskID() const -> int {
    if (m_arguments.size() < 2) {
        throw std::runtime_error("update command need ID task and description");
    }
    auto id = std::atoi(m_arguments[1].c_str());
    if (id < 1) {
        throw std::runtime_error("invalid id argument must be int");
    }
    return id;
}

AddCommand::AddCommand(std::vector<std::string> args)
    : Command(std::move(args)) {}

void AddCommand::Execute() {}

UpdateCommand::UpdateCommand(std::vector<std::string> args)
    : Command(std::move(args)) {}

void UpdateCommand::Execute() {
    if (m_arguments.size() > 2) {
        auto id = GetTaskID();
        auto task = m_task_list->GetTasksById(id);
        if (task.has_value()) {
            auto t = task.value();
            t.description = m_arguments[2];
            m_task_list->Update(t);
        }
        m_task_list->Save();
    }
}

DeleteCommand::DeleteCommand(std::vector<std::string> args)
    : Command(std::move(args)) {}

void DeleteCommand::Execute() {
    std::cout << "Delete command TBD\n";
}
MarkIsCommand::MarkIsCommand(std::vector<std::string> args)
    : Command(std::move(args)) {}

void MarkIsCommand::MarkIs(const TaskStatus &status) const {
    if (m_arguments.size() > 1) {
        auto id = GetTaskID();
        auto task = m_task_list->GetTasksById(id);
        if (task.has_value()) {
            auto t = task.value();
            t.status = status;
            m_task_list->Update(t);
        }
        m_task_list->Save();
    }
}

MarkIsDoneCommand::MarkIsDoneCommand(std::vector<std::string> args)
    : MarkIsCommand(std::move(args)) {}

void MarkIsDoneCommand::Execute() {
    MarkIs(TaskStatus::DONE);
}

MarkTodoCommand::MarkTodoCommand(std::vector<std::string> args)
    : MarkIsCommand(std::move(args)) {}

void MarkTodoCommand::Execute() {
    MarkIs(TaskStatus::TODO);
}
MarkInProgressCommand::MarkInProgressCommand(std::vector<std::string> args)
    : MarkIsCommand(std::move(args)) {}

void MarkInProgressCommand::Execute() {
    MarkIs(TaskStatus::IN_PROGRESS);
}

ListCommand::ListCommand(std::vector<std::string> args)
    : Command(std::move(args)) {}

void ListCommand::PrintTasks(const std::vector<Task> &tasks) noexcept {
    for (const auto &t : tasks) {
        t.Print(std::cout);
        std::cout << '\n';
    }
}

void ListCommand::Execute() {
    std::vector<Task> tasks;
    if (m_arguments.size() > 1) {
        auto status = SUPPORTED_LIST_FILTERS.find(m_arguments[1]);
        if (status != SUPPORTED_LIST_FILTERS.end()) {
            tasks = m_task_list->GetTasksByStatus(status->second);
        }
    } else {
        tasks = m_task_list->GetAll();
    }

    if (tasks.empty()) {
        std::cout << "No tasks\n";
    } else {
        // std::cout << "ID\tCREATTION\t\t\tSTATUS\tDESCRIPITION\t\tREDACT\n---\n";
        for (const auto &t : tasks) {
            t.Print(std::cout);
            std::cout << '\n';
        }
        std::cout << "---\nCOUNT TASKS: \n"
                  << "TODO:\t\t" << m_task_list->CountBy(TaskStatus::TODO) << "\nIN PROGRESS:\t"
                  << m_task_list->CountBy(TaskStatus::IN_PROGRESS) << "\nDONE:\t\t"
                  << m_task_list->CountBy(TaskStatus::DONE) << "\n";
    }
}
