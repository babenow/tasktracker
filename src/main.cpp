#include "commands.hpp"
#include "task.hpp"
#include "task_list.hpp"
#include <chrono>
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
    {"add", Command::ADD},
    {"update", Command::UPDATE},
    {"delete", Command::DELETE},
    {"mark-in-progress", Command::MARK_AS_TODO},
    {"mark-is-done", Command::MARK_AS_DONE},
    {"list", Command::LIST},
};

auto parseCommandQuery(int argc, const char **argv) -> Cmd {
    Cmd command;
    if (argc == 1) {
        std::stringstream ss;
        for (auto i = COMMAND_LIST.begin(); i != COMMAND_LIST.end(); i++) {
            if (i != COMMAND_LIST.begin()) {
                ss << ", ";
            }
            ss << i->first;
        }
        auto msg = "Command is empty. The program support commands: " + ss.str();
        throw std::runtime_error(msg);
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
    case Command::ADD:
        command = std::make_shared<AddCommand>(cmd.second);
        break;
    case Command::UPDATE:
        command = std::make_shared<UpdateCommand>(cmd.second);
        break;
    case Command::DELETE:
        command = std::make_shared<DeleteCommand>(cmd.second);
    case Command::MARK_AS_DONE:
        command = std::make_shared<MarkAsDoneCommand>(cmd.second);
        break;
    case Command::MARK_AS_TODO:
        command = std::make_shared<MarkAsTodoCommand>(cmd.second);
        break;
    case Command::LIST:
        command = std::make_shared<ListCommand>(cmd.second);
        break;
    }
    return command;
}

auto main(int argc, char const **argv) -> int {
    setlocale(LC_ALL, "");

    try {
        auto cmd = parseCommandQuery(argc, argv);
        auto command = makeCommandPointer(cmd);
        auto tasks = std::filesystem::current_path().append("tasks.json");
        auto tasks_list = TaskList(tasks.c_str());
        tasks_list.Add({
            -1,
            "Add",
            TaskStatus::TODO,
            std::chrono::system_clock::now(),
            std::chrono::system_clock::now(),
        });
        tasks_list.Save();
        try {
            command->Execute();
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
