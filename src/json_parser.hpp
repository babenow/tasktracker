#pragma once

#include "task.hpp"
#include <cctype>
#include <chrono>
#include <string>
#include <string_view>
#include <vector>
class JSONParser {
    std::string m_input;
    size_t m_pos = 0;

public:
    explicit JSONParser(std::string_view input);
    JSONParser() = default;

    inline void setData(std::string_view data) { m_input = data; }

    inline auto peek() const -> char { return m_pos < m_input.size() ? m_input[m_pos] : '\0'; };
    inline auto get() { return m_pos < m_input.size() ? m_input[m_pos++] : '\0'; }
    inline void skipWhitespace() {
        while (std::isspace(peek())) {
            get();
        }
    }

    auto parseString() -> std::string;
    auto parseInt() -> int;
    auto parseDateTime() -> std::chrono::system_clock::time_point;
    auto parseStatus() -> TaskStatus;
    auto parseTask() -> Task;
    auto parseTaskList() -> std::vector<Task>;
};
