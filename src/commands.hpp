#pragma once

#include "task_list.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
class Command {
protected:
    std::vector<std::string> m_arguments;
    std::shared_ptr<TaskList> m_task_list;

public:
    virtual ~Command() = default;
    enum SupportCommands : std::uint8_t { ADD, UPDATE, DELETE, MARK_AS_DONE, MARK_AS_TODO, LIST };
    explicit Command(std::vector<std::string> args);
    virtual void Execute() = 0;
    auto GetTaskID() const -> int;
    void SetTaskList(std::shared_ptr<TaskList> task_list) { m_task_list = task_list; }
};

class AddCommand : public Command {
public:
    explicit AddCommand(std::vector<std::string> args);
    ~AddCommand() override = default;
    void Execute() override;
};

class UpdateCommand : public Command {
public:
    explicit UpdateCommand(std::vector<std::string> args);
    ~UpdateCommand() override = default;
    void Execute() override;
};

class DeleteCommand : public Command {
public:
    explicit DeleteCommand(std::vector<std::string> args);
    ~DeleteCommand() override = default;
    void Execute() override;
};

class MarkAsDoneCommand : public Command {
public:
    explicit MarkAsDoneCommand(std::vector<std::string> args);
    ~MarkAsDoneCommand() override = default;
    void Execute() override;
};

class MarkAsTodoCommand : public Command {
public:
    explicit MarkAsTodoCommand(std::vector<std::string> args);
    ~MarkAsTodoCommand() override = default;
    void Execute() override;
};

class ListCommand : public Command {
public:
    explicit ListCommand(std::vector<std::string> args);
    ~ListCommand() override = default;
    void PrintTasks(std::vector<Task> tasks) const noexcept;
    void Execute() override;
};
