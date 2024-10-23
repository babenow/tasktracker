#include "commands.hpp"
#include "task_list.hpp"
#include <clocale>
#include <filesystem>
#include <format>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

// enum Command : int { ADD, UPDATE, DELETE, MARK_IN_PROGRESS, MARK_IS_DONE, LIST };
using CommandsMap = std::unordered_map<std::string, Command::SupportCommands>;
using Cmd = std::pair<Command::SupportCommands, std::vector<std::string>>;

const CommandsMap COMMAND_LIST = {
    {"add", Command::ADD_TASK},
    {"update", Command::UPDATE_TASK},
    {"delete", Command::DELETE_TASK},
    {"mark-in-progress", Command::MARK_IN_PROGRESS},
    {"mark-progress", Command::MARK_IN_PROGRESS},
    {"mark-is-done", Command::MARK_AS_DONE},
    {"mark-done", Command::MARK_AS_DONE},
    {"mark-todo", Command::MARK_AS_TODO},
    {"mark-as-todo", Command::MARK_AS_TODO},
    {"list", Command::LIST},
};

auto parseCommandQuery(int argc, const char **argv) -> Cmd {
    Cmd command;
    if (argc == 1) {
        std::stringstream ss;
        for (auto i = COMMAND_LIST.begin(); i != COMMAND_LIST.end(); ++i) {
            if (i != COMMAND_LIST.begin()) {
                ss << ", ";
            }
            ss << i->first;
        }
        const auto MSG = "Command is empty. The program support commands: " + ss.str();
        throw std::runtime_error(MSG);
    }

    auto got = COMMAND_LIST.find(argv[1]);
    if (got == COMMAND_LIST.end()) {
        throw std::runtime_error(std::format("command \"{}\" not registered", argv[1]));
    }
    command.first = got->second;
    std::vector<std::string> arguments;
    for (int i = 1; i < argc; i++) {
        arguments.emplace_back(argv[i]);
    }
    command.second = arguments;

    return command;
}

auto makeCommandPointer(Cmd cmd) -> std::shared_ptr<Command> {
    std::shared_ptr<Command> command;
    switch (cmd.first) {
    case Command::ADD_TASK:
        command = std::make_shared<AddCommand>(cmd.second);
        break;
    case Command::UPDATE_TASK:
        command = std::make_shared<UpdateCommand>(cmd.second);
        break;
    case Command::DELETE_TASK:
        command = std::make_shared<DeleteCommand>(cmd.second);
        break;
    case Command::MARK_AS_DONE:
        command = std::make_shared<MarkIsDoneCommand>(cmd.second);
        break;
    case Command::MARK_AS_TODO:
        command = std::make_shared<MarkTodoCommand>(cmd.second);
        break;
    case Command::LIST:
        command = std::make_shared<ListCommand>(cmd.second);
        break;
    case Command::MARK_IN_PROGRESS:
        command = std::make_shared<MarkInProgressCommand>(cmd.second);
        break;
    }
    return command;
}

auto main(const int argc, char const **argv) -> int {
    setlocale(LC_ALL, "");

    try {
        const auto CMD = parseCommandQuery(argc, argv);
        const auto COMMAND = makeCommandPointer(CMD);
        const auto TASKS_PATH = std::filesystem::path(argv[0]).parent_path().append("tasks.json");
        const auto TASKS_LIST = std::make_shared<TaskList>(TASKS_PATH.c_str());
        try {
            COMMAND->SetTaskList(TASKS_LIST);
            system("clear");
            COMMAND->Execute();
        } catch (std::runtime_error e) {
            std::cout << "Command execution error: " << e.what() << "\n";
            return 1;
        }
    } catch (std::runtime_error e) {
        std::cout << e.what() << "\n";
        return 1;
    }

    return 0;
}
