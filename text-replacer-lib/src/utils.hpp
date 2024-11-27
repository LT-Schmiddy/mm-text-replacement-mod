#pragma once

#define MESSAGE_BUFFER_SIZE 1280
#define MESSAGE_HEADER_SIZE 11
#define MESSAGE_CONTENT_SIZE MESSAGE_BUFFER_SIZE - MESSAGE_HEADER_SIZE


#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>
namespace fs = std::filesystem;
namespace ns = nlohmann;

namespace text_replacer_lib::utils {
    bool is_ascii(char c);
    // template <typename T> std::string to_hex(T* bytes, int size);
    std::string to_hex(void* bytes, int size);
    void from_hex(std::string p_str, void* p_bytes, int p_size);
    fs::path get_lib_path();
}
