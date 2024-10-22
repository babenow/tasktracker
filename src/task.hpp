#pragma once

#include "helper.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>

enum class TaskStatus { TODO, IN_PROGRESS, DONE, UNKNOWN };
static const std::unordered_map<TaskStatus, std::string> STATUS_MAP = {
    {TaskStatus::TODO, "todo"},
    {TaskStatus::IN_PROGRESS, "in_progress"},
    {TaskStatus::DONE, "done"},
};
struct Task {
    mutable int id;
    std::optional<std::string> description;
    TaskStatus status;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    auto ToJsonString() const -> std::string {
        std::stringstream ss("");
        ss << "{";
        ss << "\"id\":" << id << ",";
        ss << "\"description\":" << "\"" << (description.has_value() ? description.value() : "")
           << "\",";
        ss << "\"status\":" << "\"" << Task::statusToString(status) << "\",";
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

    void Print(std::ostream &str) const {
        auto time = std::chrono::system_clock::to_time_t(created_at);
        str << "[" << id << "]\t";
        str << std::put_time(std::localtime(&time), "%d.%m.%Y, %H:%M");
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
        str << description.value_or("-Task without description-") << "\t";
        if (created_at != updated_at) {
            str << " Red: ";
            time = std::chrono::system_clock::to_time_t(updated_at);
            str << std::put_time(std::localtime(&time), "%d.%m.%Y, %H:%M");
        }
    }
};
;
