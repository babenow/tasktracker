#pragma once

#include "helper.hpp"
#include <chrono>
#include <sstream>
#include <string>
#include <unordered_map>

enum class TaskStatus { TODO, IN_PROGRESS, DONE, UNKNOWN };
static const std::unordered_map<TaskStatus, std::string> STATUS_MAP = {
    {TaskStatus::TODO, "todo"},
    {TaskStatus::IN_PROGRESS, "in_progress"},
    {TaskStatus::DONE, "completed"},
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
        ss << "\"description\":" << "\"" << (description.has_value() ? description.value() : "")
           << "\",";
        ss << "\"status\":" << "\"" << Task::statusToString(status) << "\",";
        ss << "\"created_at\":" << "\"" << TimePointToString(created_at) << "\",";
        ss << "\"updated_at\":" << "\"" << TimePointToString(updated_at) << "\"";
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

    void PrintTask() {}
};
;
