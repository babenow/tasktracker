#pragma once

#include "helper.hpp"
#include <chrono>
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

enum class TaskStatus : int8_t { TODO, IN_PROGRESS, DONE, UNKNOWN };
inline auto to_string(const TaskStatus E) -> const char * {
    switch (E) {
    case TaskStatus::TODO:
        return "todo";
    case TaskStatus::IN_PROGRESS:
        return "in_progress";
    case TaskStatus::DONE:
        return "done";
    default:
        return "unknown";
    }
}

static const std::unordered_map<TaskStatus, std::string> STATUS_MAP = {
    {TaskStatus::TODO, "todo"},
    {TaskStatus::IN_PROGRESS, "in_progress"},
    {TaskStatus::DONE, "done"},
};
struct Task {
    int id;
    std::optional<std::string> description;
    TaskStatus status;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    auto ToJsonString() const -> std::string {
        std::stringstream ss("");
        ss << "{";
        ss << "\"id\":" << id << ",";
        ss << "\"description\":" << "\"" << description.value_or("") << "\",";
        ss << "\"status\":" << "\"" << to_string(status) << "\",";
        ss << "\"createdAt\":" << "\"" << TimePointToString(created_at) << "\",";
        ss << "\"updatedAt\":" << "\"" << TimePointToString(updated_at) << "\"";
        ss << "}";
        return ss.str();
    };

    static auto statusToString(TaskStatus status) -> std::string {
        auto got = STATUS_MAP.find(status);
        return got != STATUS_MAP.end() ? got->second : "unknown";
    }

    static auto statusFromString(std::string_view status) -> TaskStatus {
        for (const auto &[tse, tss] : STATUS_MAP) {
            if (tss == status.data()) {
                return tse;
            }
        }

        return TaskStatus::UNKNOWN;
    }

    void BeforeUpdate() { updated_at = std::chrono::system_clock::now(); }
    void BeforeCreate() {
        created_at = std::chrono::system_clock::now();
        BeforeUpdate();
    }

    void PrintCreatedTime(std::ostream &str, const std::string &format = "%d.%m.%Y, %H:%M") const {
        auto time = std::chrono::system_clock::to_time_t(created_at);
        PrintTime(str, time, format);
    }

    void PrintUpdatedTime(std::ostream &str, const std::string &format = "%d.%m.%Y, %H:%M") const {
        auto time = std::chrono::system_clock::to_time_t(updated_at);
        PrintTime(str, time, format);
    }

    void Print(std::ostream &str) const {
        str << "[" << id << "]\t";
        PrintCreatedTime(str);
        str << "\t[";
        switch (status) {
        case TaskStatus::TODO:
            str << " ";
            break;
        case TaskStatus::IN_PROGRESS:
            str << "P";
            break;
        case TaskStatus::DONE:
            str << "x";
            break;
        case TaskStatus::UNKNOWN:
            str << "-";
            break;
        }
        str << "]\t\t";
        auto desc = description.value_or("Task without description");
        str << desc;
        if (created_at != updated_at) {
            str << " [Red: ";
            PrintUpdatedTime(str);
            str << "]";
        }
    }

private:
    static void PrintTime(std::ostream &str,
                          time_t time,
                          const std::string &format = "%d.%m.%Y, %H:%M") {
        str << std::put_time(std::localtime(&time), format.c_str());
    }
};
;
