#pragma once

#include <string>
#include <stdexcept>
#include "RuneLogger.hpp"

namespace RuneLang {

class RuneError : public std::runtime_error {
public:
    enum class ErrorCode {
        NONE = 0,
        SYSTEM_ERROR,
        MEMORY_ERROR,
        FILE_ERROR,
        PROCESS_ERROR,
        NETWORK_ERROR,
        SECURITY_ERROR,
        UNKNOWN_ERROR
    };

    RuneError(ErrorCode code, const std::string& message)
        : std::runtime_error(message), code_(code) {
        LOG_ERROR("Exception: ", getErrorString(code), " - ", message);
    }

    ErrorCode getCode() const { return code_; }

    static std::string getErrorString(ErrorCode code) {
        switch (code) {
            case ErrorCode::NONE: return "No error";
            case ErrorCode::SYSTEM_ERROR: return "System error";
            case ErrorCode::MEMORY_ERROR: return "Memory error";
            case ErrorCode::FILE_ERROR: return "File error";
            case ErrorCode::PROCESS_ERROR: return "Process error";
            case ErrorCode::NETWORK_ERROR: return "Network error";
            case ErrorCode::SECURITY_ERROR: return "Security error";
            case ErrorCode::UNKNOWN_ERROR:
            default: return "Unknown error";
        }
    }

private:
    ErrorCode code_;
};

#define RUNE_THROW(code, message) \
    throw RuneError(code, message)

} // namespace RuneLang
