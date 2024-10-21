#pragma once

#include <cstdint>
#include <string>
#include <vector>
class Command {
protected:
    std::vector<std::string> m_arguments;

public:
    enum SupportCommands : std::uint8_t { ADD, UPDATE, DELETE, MARK_AS_DONE, MARK_AS_TODO, LIST };
    explicit Command(std::vector<std::string> args);
    virtual void Execute() = 0;
    auto GetTaskID() const -> int;
};

class AddCommand : public Command {
public:
    explicit AddCommand(std::vector<std::string> args);
    virtual ~AddCommand() = default;
    void Execute() override;
};

class UpdateCommand : public Command {
public:
    explicit UpdateCommand(std::vector<std::string> args);
    virtual ~UpdateCommand() = default;
    void Execute() override;
};

class DeleteCommand : public Command {
public:
    explicit DeleteCommand(std::vector<std::string> args);
    virtual ~DeleteCommand() = default;
    void Execute() override;
};

class MarkAsDoneCommand : public Command {
public:
    explicit MarkAsDoneCommand(std::vector<std::string> args);
    virtual ~MarkAsDoneCommand() = default;
    void Execute() override;
};

class MarkAsTodoCommand : public Command {
public:
    explicit MarkAsTodoCommand(std::vector<std::string> args);
    virtual ~MarkAsTodoCommand() = default;
    void Execute() override;
};

class ListCommand : public Command {
public:
    explicit ListCommand(std::vector<std::string> args);
    virtual ~ListCommand() = default;
    void Execute() override;
};
