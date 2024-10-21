#include "json_parser.hpp"
#include "helper.hpp"
#include "task.hpp"
#include <cctype>
#include <chrono>
#include <stdexcept>
#include <string>
#include <vector>

JSONParser::JSONParser(std::string_view input)
    : m_input(input) {}

auto JSONParser::parseString() -> std::string {
    std::string result{};
    get();
    while (peek() != '"') {
        if (peek() == '\\') {
            get();
            switch (get()) {
            case 'n':
                result += '\n';
                break;
            case 'r':
                result += '\r';
                break;
            case 't':
                result += '\t';
                break;
            case 'b':
                result += '\b';
                break;
            case 'f':
                result += '\f';
                break;
            case '"':
                result += '"';
                break;
            case '\\':
                result += '\\';
                break;
            default:
                throw std::runtime_error("Invalid escaped sequence");
            }
        } else {
            result += get();
        }
    }
    get();
    return result;
}

auto JSONParser::parseInt() -> int {
    size_t start = m_pos;
    while (isdigit(peek())) {
        get();
    }
    return std::stoi(std::string(m_input.substr(start, m_pos - start)));
}

auto JSONParser::parseDateTime() -> std::chrono::system_clock::time_point {
    auto date_time_str = parseString();
    return StringToTimePoint(date_time_str);
}

auto JSONParser::parseStatus() -> TaskStatus {
    auto status_str = parseString();
    return Task::statusFromString(status_str);
}

auto JSONParser::parseTask() -> Task {
    Task task;
    skipWhitespace();
    if (get() != '{') {
        throw std::runtime_error("Expected '{' at start of task");
    }

    while (true) {
        skipWhitespace();
        if (peek() == '}') {
            get();
            return task;
        }

        auto key = parseString();
        skipWhitespace();
        if (get() != ':') {
            throw std::runtime_error("Expected ':' after key in task");
        }
        skipWhitespace();

        if (key == "id") {
            task.id = parseInt();
        } else if (key == "description") {
            task.description = parseString();
        } else if (key == "status") {
            task.status = parseStatus();
        } else if (key == "created_at") {
            task.created_at = parseDateTime();
        } else if (key == "updated_at") {
            task.updated_at = parseDateTime();
        } else {
            throw std::runtime_error("Unknown key in task: " + key);
        }

        skipWhitespace();
        if (peek() == '}') {
            get();
            return task;
        }

        if (get() != ',') {
            throw std::runtime_error("Expected ',' or '}' in task");
        }
    }
}

auto JSONParser::parseTaskList() -> std::vector<Task> {
    std::vector<Task> tasks;
    skipWhitespace();
    if (get() != '[') {
        throw std::runtime_error("Expected '[' at start of task list");
    }

    while (true) {
        skipWhitespace();
        if (peek() == ']') {
            get();
            return tasks;
        }

        tasks.emplace_back(parseTask());

        skipWhitespace();
        if (peek() == ']') {
            get();
            return tasks;
        }
        if (get() != ',') {
            throw std::runtime_error("Expected ',' or ']' in task list");
        }
    }

    return tasks;
}
