#pragma once

#include <chrono>
#include <string>

static inline auto TimePointToString(const std::chrono::system_clock::time_point &tp) -> std::string {
    auto tt = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&tt);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Helper function to convert string to time_point
static inline auto StringToTimePoint(const std::string &s) -> std::chrono::system_clock::time_point {
    std::tm tm = {};
    std::istringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}
