#include "hw_prod5.h"
#include <iostream>
#include <ctime>
#include <iomanip>

Log* Log::Instance() {
    static Log instance;
    return &instance;
}

void Log::message(LogLevel level, const std::string& message) {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << ltm->tm_mday << "/"
       << std::setw(2) << std::setfill('0') << 1 + ltm->tm_mon << "/"
       << 1900 + ltm->tm_year << " "
       << std::setw(2) << std::setfill('0') << ltm->tm_hour << ":"
       << std::setw(2) << std::setfill('0') << ltm->tm_min << ":"
       << std::setw(2) << std::setfill('0') << ltm->tm_sec;

    entries.push_back({level, ss.str() + ": " + message});

    if (entries.size() > maxEntries) {
        entries.erase(entries.begin());
    }
}

void Log::print() const {
    for (const auto& entry : entries) {
        switch (entry.first) {
            case LOG_NORMAL:
                std::cout << "NORMAL: ";
                break;
            case LOG_WARNING:
                std::cout << "WARNING: ";
                break;
            case LOG_ERROR:
                std::cout << "ERROR: ";
                break;
        }
        std::cout << entry.second << std::endl;
    }
}