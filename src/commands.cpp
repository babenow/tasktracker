#include "commands.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

Command::Command(std::vector<std::string> args)
    : m_arguments(std::move(args)){};

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

void ListCommand::Execute() {
    std::cout << "List Command: TBD\n";
}
