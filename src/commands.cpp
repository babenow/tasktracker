#include "commands.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

Command::Command(std::vector<std::string> args)
    : m_arguments(std::move(args)) {};

auto Command::GetTaskID() const -> int {
    if (m_arguments.empty()) {
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
    std::cout << "Updated Task: \nID: " << GetTaskID() << "\n";
    std::cout << "Update Command EXECUTE: TBD";
}

DeleteCommand::DeleteCommand(std::vector<std::string> args)
    : Command(std::move(args)) {}

void DeleteCommand::Execute() {
    std::cout << "Delete command TBD\n";
}

MarkAsDoneCommand::MarkAsDoneCommand(std::vector<std::string> args)
    : Command(std::move(args)) {}

void MarkAsDoneCommand::Execute() {
    std::cout << "Mark as done: TBD\n";
}

MarkAsTodoCommand::MarkAsTodoCommand(std::vector<std::string> args)
    : Command(std::move(args)) {}

void MarkAsTodoCommand::Execute() {
    std::cout << "Mark as TODO: TBD\n";
}

ListCommand::ListCommand(std::vector<std::string> args)
    : Command(std::move(args)) {}

void ListCommand::PrintTasks(std::vector<Task> tasks) const noexcept {
    for (const auto &t : tasks) {
        t.Print(std::cout);
        std::cout << '\n';
    }
}

void ListCommand::Execute() {
    std::vector<Task> tasks;
    if (m_arguments.size() > 1) {
        if (m_arguments[1] == "done") {
            tasks = m_task_list->GetTasksByStatus(TaskStatus::DONE);
        } else if (m_arguments[1] == "todo") {
            tasks = m_task_list->GetTasksByStatus(TaskStatus::TODO);
        } else if (m_arguments[1] == "in-progress") {
            tasks = m_task_list->GetTasksByStatus(TaskStatus::IN_PROGRESS);
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
