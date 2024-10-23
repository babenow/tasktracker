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
    enum SupportCommands : std::uint8_t { ADD, UPDATE, DELETE, MARK_IN_PROGRESS, MARK_AS_DONE, MARK_AS_TODO, LIST };
    explicit Command(std::vector<std::string> args);
    virtual void Execute() = 0;
    auto GetTaskID() const -> int;
    void SetTaskList(std::shared_ptr<TaskList> task_list) { m_task_list = task_list; }
};

class AddCommand final : public Command {
public:
    explicit AddCommand(std::vector<std::string> args);
    ~AddCommand() override = default;
    void Execute() override;
};

class UpdateCommand final : public Command {
public:
    explicit UpdateCommand(std::vector<std::string> args);
    ~UpdateCommand() override = default;
    void Execute() override;
};

class DeleteCommand final : public Command {
public:
    explicit DeleteCommand(std::vector<std::string> args);
    ~DeleteCommand() override = default;
    void Execute() override;
};

class MarkIsCommand : public Command {
public:
    explicit MarkIsCommand(std::vector<std::string> args);
    ~MarkIsCommand() override = default;
    void Execute() override = 0;
    void MarkIs(const TaskStatus& status) const;
};

class MarkIsDoneCommand final : public MarkIsCommand {
public:
    explicit MarkIsDoneCommand(std::vector<std::string> args);
    ~MarkIsDoneCommand() override = default;
    void Execute() override;
};

class MarkTodoCommand final : public MarkIsCommand {
public:
    explicit MarkTodoCommand(std::vector<std::string> args);
    ~MarkTodoCommand() override = default;
    void Execute() override;
};

class MarkInProgressCommand final : public MarkIsCommand {
public:
    explicit MarkInProgressCommand(std::vector<std::string> args);
    ~MarkInProgressCommand() override = default;
    void Execute() override;
};

class ListCommand final : public Command {
public:
    explicit ListCommand(std::vector<std::string> args);
    ~ListCommand() override = default;
    static void PrintTasks(const std::vector<Task> &tasks) noexcept;
    void Execute() override;
};
